#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <string.h>
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"

//*********logica de GPIO***********
#define logica "Negativa"
// #define logica "Positiva"

uint8_t TRUE;
uint8_t FALSE;

#define sppButton GPIO_NUM_23
#define led0 GPIO_NUM_2

uint8_t sppButtonPressed = 0;
uint8_t sppButtonMQTT = 0;
//*********logica de GPIO***********

//*********Maquina de estado***********
uint8_t estadoActual = 0;
uint8_t estadoAnterior = 99; // valor para que se inicialice la comunicacion serial, luego se va a igualar a estado actual

//*********Wifi***********
static const char *TAG = "Proyecto Final";
#define WIFI_SSID "Nexxt"         
#define WIFI_PASSWORD "ab123456cd" 
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
static EventGroupHandle_t wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "Conexión fallida, intentando reconectar...");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Conectado con IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    wifi_event_group = xEventGroupCreate();

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Configuración Wi-Fi completada. Intentando conectar...");

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Conexión Wi-Fi exitosa.");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Conexión Wi-Fi fallida.");
    }
    else
    {
        ESP_LOGE(TAG, "Error inesperado en el evento Wi-Fi.");
    }
}

//*********parametros MQTT**************
#define CONFIG_BROKER_URL "mqtt://broker.hivemq.com"

void inicializarGPIO(void);

// MQTT*********************

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id)
    {

    case MQTT_EVENT_CONNECTED:
        msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/SPP", 1);
        break;

    case MQTT_EVENT_DATA:

        if (strncmp(event->topic, "/2022-1151/SPP", event->topic_len) == 0)
        {
            char received_data[2]; // Solo esperamos "0" o "1"
            snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);
            if (strcmp(received_data, "1") == 0)
            {
                sppButtonMQTT = 1;
            }
        }
        break;

    case MQTT_EVENT_ERROR:
        // ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;

    default:
        // ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt5_app_start(void)
{
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = false,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void maquina_estado(void *pvParameters) // task maquina estado
{
    while (1)
    {
        if (sppButtonPressed == 0) // condicion para solo leer un pulso
        {
            if (gpio_get_level(sppButton) == TRUE) // boton fisico
            {
                sppButtonPressed = 1;
                if (estadoActual < 4)
                {
                    estadoActual += 1;
                }
                else
                    estadoActual = 0;
            }
            else if (sppButtonMQTT == 1) // boton MQTT
            {
                sppButtonMQTT = 0; // aseguramos estado
                if (estadoActual < 4)
                {
                    estadoActual += 1;
                }
                else
                    estadoActual = 0;
            }
        }

        if (gpio_get_level(sppButton) == FALSE) // si no esta presionado reinicia la condicion
        {
            sppButtonPressed = 0;
        }
        // Delay de 100 ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void info_serial(void *pvParameters) // task informacion serial
{
    while (1)
    {
        if (estadoAnterior != estadoActual)
        {
            ESP_LOGI("Info Serial", "Estado = %d", estadoActual);
            estadoAnterior = estadoActual;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void led_blink(void *pvParameters) // task parpadeo led
{

    uint16_t contadorActual = 0;
    uint8_t ledLevel = 0;
    uint8_t estadoLedAnterior = 0; // variable para reiniciar los contadores de tiempo
    uint16_t contadorBarrido = 0;
    uint16_t tiempoBarrido = 0;

    while (1)
    {
        if (estadoLedAnterior != estadoActual)
        {
            estadoLedAnterior = estadoActual;
            contadorActual = 0; // reiniciar el contador en cambio de estado
            tiempoBarrido = 0;  // reiniciar el contador en cambio de estado
            ledLevel = 1;
        }

        switch (estadoActual)
        {
        case 1:
            if (contadorActual >= 500)
            {
                contadorActual = 0;
                ledLevel = !ledLevel;
                gpio_set_level(led0, ledLevel);
            }

            contadorActual += 10; // suma 10 ms al contador
            break;

        case 2:
            if (contadorActual >= 100)
            {
                contadorActual = 0;
                ledLevel = !ledLevel;
                gpio_set_level(led0, ledLevel);
            }

            contadorActual += 10; // suma 10 ms al contador
            break;

        case 3:
            if (contadorActual >= 1000)
            {
                contadorActual = 0;
                ledLevel = !ledLevel;
                gpio_set_level(led0, ledLevel);
            }

            contadorActual += 10; // suma 10 ms al contador
            break;

        case 4:
            if (tiempoBarrido >= 1000)
            {
                if (contadorBarrido > 1000) // si llega a 1 segundo reinicia el barrido a 100ms
                {
                    contadorBarrido = 100;
                }
                else
                {
                    contadorBarrido += 100; // va sumando 100ms al periodo cada 1000ms
                }

                tiempoBarrido = 0;
            }

            if (contadorActual >= contadorBarrido)
            {
                contadorActual = 0;
                ledLevel = !ledLevel;
                gpio_set_level(led0, ledLevel);
            }

            tiempoBarrido += 10;
            contadorActual += 10; // suma 10 ms al contador
            break;

        default: // estado 0 o cualquier otro estado fuera de 1 a 4
            gpio_set_level(led0, 0);
            if (contadorActual != 0)
                contadorActual = 0;
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main()
{
    // Inicializar GPIO
    inicializarGPIO();

    // Inicializar NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Inicializando Wi-Fi...");
    wifi_init_sta();

    // Iniciar MQTT
    mqtt5_app_start();

    // Crear la tarea Maquina de estado
    xTaskCreate(
        maquina_estado,
        "Maquina De Estado",
        2048,
        NULL,
        5,
        NULL);

    xTaskCreate(
        info_serial,
        "Serial",
        2048,
        NULL,
        5,
        NULL);

    xTaskCreate(
        led_blink,
        "Led blink",
        2048,
        NULL,
        5,
        NULL);
}

void inicializarGPIO()
{
    if (strcmp(logica, "Negativa") == 0)
    {
        TRUE = 0;
        FALSE = 1;
        ESP_LOGI("GPIO", "Logica Negativa");
    }
    else if (strcmp(logica, "Positiva") == 0)
    {
        TRUE = 1;
        FALSE = 0;
        ESP_LOGI("GPIO", "Logica Positiva");
    }
    else
        ESP_LOGE("GPIO", "Error Logica Incorrecta");

    // Boton SPP Logica Negativa
    gpio_reset_pin(sppButton);
    gpio_set_direction(sppButton, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sppButton, GPIO_PULLUP_ONLY);

    // Led integrado ESP32
    gpio_reset_pin(led0);
    gpio_set_direction(led0, GPIO_MODE_OUTPUT);

    ESP_LOGI("GPIO", "GPIO Inicializado");
}
