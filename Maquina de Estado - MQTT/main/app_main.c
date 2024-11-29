#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ESTADO_INIT 0
#define ESTADO_CERRADO 1
#define ESTADO_ABIERTO 2
#define ESTADO_CERRANDO 3
#define ESTADO_ABRIENDO 4
#define ESTADO_ERROR 5
#define TRUE 1
#define FALSE 0
#define RT_MAX 180
#define ERROR_OK 0
#define ERROR_LS 1
#define ERROR_RT 2

int ESTADO_SIGUIENTE = ESTADO_INIT;
int ESTADO_ACTUAL = ESTADO_INIT;
int ESTADO_ANTERIOR = ESTADO_INIT;

struct DATA_IO
{
    unsigned int LSC : 1;
    unsigned int LSA : 1;
    unsigned int SPP : 1;
    unsigned int MA : 1;
    unsigned int MC : 1;
    unsigned int CONT_RT;
    unsigned int LED_A : 1;
    unsigned int LED_C : 1;
    unsigned int LED_ERR : 1;
    unsigned int COD_ERR : 1;
} data_io;

static const char *TAG = "Puerta Garaje";

// static void log_error_if_nonzero(const char *message, int error_code)
// {
//     if (error_code != 0)
//     {
//         ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
//     }
// }

// static char last_message[256] = ""; // Almacena el último mensaje recibido para detectar cambios

// Función de evento MQTT
static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/LSC", 1);
        ESP_LOGI(TAG, "Subscribed to /2022-1151/LSC, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/LSA", 1);
        ESP_LOGI(TAG, "Subscribed to /2022-1151/LSA, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/SPP", 1);
        ESP_LOGI(TAG, "Subscribed to /2022-1151/SPP, msg_id=%d", msg_id);

        // msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/MA", 1);
        // ESP_LOGI(TAG, "Subscribed to /2022-1151/MA, msg_id=%d", msg_id);

        // msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/MC", 1);
        // ESP_LOGI(TAG, "Subscribed to /2022-1151/MC, msg_id=%d", msg_id);

        break;

    case MQTT_EVENT_DATA:
        // ESP_LOGI(TAG, "MQTT_EVENT_DATA");

        // // Verifica si el mensaje recibido es del tópico `2022-1151/puerta`
        // if (strncmp(event->topic, "/2022-1151/LSC", event->topic_len) == 0)
        // {
        //     char new_message[256];
        //     snprintf(new_message, sizeof(new_message), "%.*s", event->data_len, event->data);

        //     // Compara con el último mensaje recibido para detectar cambios
        //     if (strcmp(new_message, last_message) != 0)
        //     {
        //         // Si hay un cambio, registra el nuevo mensaje y lo muestra
        //         ESP_LOGI(TAG, "Cambio detectado en /2022-1151/puerta: %s", new_message);
        //         strncpy(last_message, new_message, sizeof(last_message)); // Actualiza el último mensaje
        //     }
        // }

        if (strncmp(event->topic, "/2022-1151/LSC", event->topic_len) == 0)
        {
            char received_data[2]; // Solo esperamos "0" o "1"
            snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);

            if (strcmp(received_data, "1") == 0)
            {
                data_io.LSC = TRUE;
                ESP_LOGI(TAG, "LSC = 1");
            }
            else if (strcmp(received_data, "0") == 0)
            {
                data_io.LSC = FALSE;
                ESP_LOGI(TAG, "LSC = 0");
            }
        }

        if (strncmp(event->topic, "/2022-1151/LSA", event->topic_len) == 0)
        {
            char received_data[2]; // Solo esperamos "0" o "1"
            snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);

            if (strcmp(received_data, "1") == 0)
            {
                data_io.LSA = TRUE;
                ESP_LOGI(TAG, "LSA = 1");
            }
            else if (strcmp(received_data, "0") == 0)
            {
                data_io.LSA = FALSE;
                ESP_LOGI(TAG, "LSA = 0");
            }
        }

        if (strncmp(event->topic, "/2022-1151/SPP", event->topic_len) == 0)
        {
            char received_data[2]; // Solo esperamos "0" o "1"
            snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);

            if (strcmp(received_data, "1") == 0)
            {
                data_io.SPP = TRUE;
                ESP_LOGI(TAG, "SPP");
            }
            // else if (strcmp(received_data, "0") == 0)
            // {
            //     ESP_LOGI(TAG, "LSA = 0");

            // }
        }
        // if (strncmp(event->topic, "/2022-1151/MA", event->topic_len) == 0)
        // {
        //     char received_data[2]; // Solo esperamos "0" o "1"
        //     snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);

        //     if (strcmp(received_data, "1") == 0)
        //     {
        //         ESP_LOGI(TAG, "Motor abrir = 1");
        //     }
        //     else if (strcmp(received_data, "0") == 0)
        //     {
        //         ESP_LOGI(TAG, "Motor abrir = 0");
        //     }
        // }
        // if (strncmp(event->topic, "/2022-1151/MC", event->topic_len) == 0)
        // {
        //     char received_data[2]; // Solo esperamos "0" o "1"
        //     snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);

        //     if (strcmp(received_data, "1") == 0)
        //     {
        //         ESP_LOGI(TAG, "Motor cerrar = 1");
        //     }
        //     else if (strcmp(received_data, "0") == 0)
        //     {
        //         ESP_LOGI(TAG, "Motor cerrar = 0");
        //     }
        // }
        // if (strncmp(event->topic, "/2022-1151/LED_A", event->topic_len) == 0)
        // {
        // }
        // if (strncmp(event->topic, "/2022-1151/LED_C", event->topic_len) == 0)
        // {
        // }
        // if (strncmp(event->topic, "/2022-1151/LED_ERR", event->topic_len) == 0)
        // {
        // }
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;

    default:
        //ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void publish_state(esp_mqtt_client_handle_t client)
{
    int msg_id;

    char mc_value[2];
    snprintf(mc_value, sizeof(mc_value), "%d", data_io.MC);
    msg_id = esp_mqtt_client_publish(client, "/2022-1151/MC", mc_value, 0, 1, 0);
    //ESP_LOGI(TAG, "Published /2022-1151/MC, msg_id=%d", msg_id);

    // Publicar estado del motor abrir (MA)
    char ma_value[2];
    snprintf(ma_value, sizeof(ma_value), "%d", data_io.MA);
    msg_id = esp_mqtt_client_publish(client, "/2022-1151/MA", ma_value, 0, 1, 0);
    //ESP_LOGI(TAG, "Published /2022-1151/MA, msg_id=%d", msg_id);

    // Publicar estado del LED A
    char led_a_value[2];
    snprintf(led_a_value, sizeof(led_a_value), "%d", data_io.LED_A);
    msg_id = esp_mqtt_client_publish(client, "/2022-1151/LED_A", led_a_value, 0, 1, 0);
    //ESP_LOGI(TAG, "Published /2022-1151/LED_A, msg_id=%d", msg_id);

    // Publicar estado del LED C
    char led_c_value[2];
    snprintf(led_c_value, sizeof(led_c_value), "%d", data_io.LED_C);
    msg_id = esp_mqtt_client_publish(client, "/2022-1151/LED_C", led_c_value, 0, 1, 0);
    //ESP_LOGI(TAG, "Published /2022-1151/LED_C, msg_id=%d", msg_id);

    // Publicar estado del LED de error
    char led_err_value[2];
    snprintf(led_err_value, sizeof(led_err_value), "%d", data_io.LED_ERR);
    msg_id = esp_mqtt_client_publish(client, "/2022-1151/LED_ERR", led_err_value, 0, 1, 0);
    //ESP_LOGI(TAG, "Published /2022-1151/LED_ERR, msg_id=%d", msg_id);
    ESP_LOGI(TAG, "Estados publicados");
}


void publish_state_task(void *pvParameters)
{
    esp_mqtt_client_handle_t client = (esp_mqtt_client_handle_t)pvParameters;

    while (1)
    {
        publish_state(client);                 // Llama a la función para publicar el estado en los tópicos
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Espera 5 segundos
    }
}

// static void mqtt5_app_start(void)
// {
//     esp_mqtt_client_config_t mqtt5_cfg = {
//         .broker.address.uri = CONFIG_BROKER_URL,
//         .session.protocol_ver = MQTT_PROTOCOL_V_5,
//         .network.disable_auto_reconnect = false,
//     };

//     esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);
//     esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
//     esp_mqtt_client_start(client);
// }
static esp_mqtt_client_handle_t mqtt5_app_start(void)
{
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = false,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client);

    return client; // Retorna el cliente MQTT
}

int FUNC_INIT(void)
{
    ESP_LOGI(TAG, "ESTADO INIT");
    ESTADO_ANTERIOR = ESTADO_INIT;
    ESTADO_ACTUAL = ESTADO_INIT;

    data_io.COD_ERR = ERROR_OK;
    data_io.CONT_RT = 0;
    data_io.MA = FALSE;
    data_io.MC = FALSE;
    data_io.LED_A = TRUE;
    data_io.LED_C = TRUE;
    data_io.LED_ERR = TRUE;
    // vTaskDelay(2000 / portTICK_PERIOD_MS);
    data_io.LED_A = FALSE;
    data_io.LED_C = FALSE;
    data_io.LED_ERR = FALSE;

    // insertar aqui funcion que verifica el estado de los sensores y LS

    while (1)
    {
        if (data_io.LSC == TRUE && data_io.LSA == FALSE)
        {
            return ESTADO_CERRADO;
        }
        if (data_io.LSC == TRUE && data_io.LSA == TRUE)
        {
            return ESTADO_ERROR;
        }
        if (data_io.LSC == FALSE && data_io.LSA == FALSE)
        {
            return ESTADO_CERRANDO;
        }
        if (data_io.LSC == FALSE && data_io.LSA == TRUE)
        {
            return ESTADO_CERRANDO;
        }
    }
}
int FUNC_ABIERTO(void)
{
    ESP_LOGI(TAG, "ESTADO ABIERTO");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABIERTO;

    data_io.MA = FALSE;
    data_io.LED_A = FALSE;
    data_io.LED_C = FALSE;
    data_io.LED_ERR = FALSE;
    data_io.SPP = FALSE; // asegurar el estado

    while (1)
    {
        if (data_io.SPP == TRUE)
        {
            data_io.SPP = FALSE;
            return ESTADO_CERRANDO;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Evita bloquear CPU
    }
}

int FUNC_CERRADO(void)
{
    ESP_LOGI(TAG, "ESTADO CERRADO");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRADO;
    data_io.MC = FALSE;
    data_io.LED_A = FALSE;
    data_io.LED_C = FALSE;
    data_io.LED_ERR = FALSE;
    data_io.SPP = FALSE; // asegurar el estado

    while (1)
    {
        if (data_io.SPP == TRUE)
        {
            data_io.SPP = FALSE;
            return ESTADO_ABRIENDO;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Evita bloquear CPU
    }
}

int FUNC_ABRIENDO(void)
{
    ESP_LOGI(TAG, "ESTADO ABRIENDO");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;
    data_io.CONT_RT = 0;
    data_io.LED_A = TRUE;
    data_io.LED_C = FALSE;
    data_io.LED_ERR = FALSE;
    data_io.SPP = FALSE; // asegurar el estado
    data_io.MC = FALSE;  // evitar enviar señal de cerrar y abrir al motor
                         // vTaskDelay(1500 / portTICK_PERIOD_MS);
    data_io.MA = TRUE;
    while (1)
    {
        if (data_io.LSA == TRUE)
        {
            return ESTADO_ABIERTO;
        }
        if (data_io.SPP == TRUE)
        {
            data_io.SPP = FALSE;
            return ESTADO_CERRANDO;
        }
        if (data_io.CONT_RT > RT_MAX)
        {
            return ESTADO_ERROR;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Evita bloquear CPU
    }
}

int FUNC_CERRANDO(void)
{
    ESP_LOGI(TAG, "ESTADO CERRANDO");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;
    data_io.CONT_RT = 0;
    data_io.LED_A = FALSE;
    data_io.LED_C = TRUE;
    data_io.LED_ERR = FALSE;
    data_io.SPP = FALSE; // asegurar el estado
    data_io.MA = FALSE;  // evitar enviar señal de cerrar y abrir al motor
    // vTaskDelay(1500 / portTICK_PERIOD_MS);
    data_io.MC = TRUE;
    while (1)
    {
        if (data_io.LSC == TRUE)
        {
            return ESTADO_CERRADO;
        }
        if (data_io.SPP == TRUE)
        {
            data_io.SPP = FALSE;
            return ESTADO_ABRIENDO;
        }
        if (data_io.CONT_RT > RT_MAX)
        {
            return ESTADO_ERROR;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Evita bloquear CPU
    }
}
int FUNC_ERROR(void)
{
    ESP_LOGI(TAG, "ESTADO ERROR");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ERROR;

    data_io.MC = FALSE;
    data_io.MC = FALSE;
    data_io.LED_A = FALSE;
    data_io.LED_C = FALSE;
    data_io.LED_ERR = TRUE;

    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS); // Evita bloquear CPU
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    //mqtt5_app_start();
    esp_mqtt_client_handle_t client = mqtt5_app_start(); // Inicia MQTT y obtiene el cliente
    xTaskCreate(&publish_state_task, "publish_state_task", 4096, (void *)client, 5, NULL);

    while (1)
    {
        if (ESTADO_SIGUIENTE == ESTADO_INIT)
        {
            ESTADO_SIGUIENTE = FUNC_INIT();
        }
        else if (ESTADO_SIGUIENTE == ESTADO_ABIERTO)
        {
            ESTADO_SIGUIENTE = FUNC_ABIERTO();
        }
        else if (ESTADO_SIGUIENTE == ESTADO_CERRADO)
        {
            ESTADO_SIGUIENTE = FUNC_CERRADO();
        }
        else if (ESTADO_SIGUIENTE == ESTADO_ABRIENDO)
        {
            ESTADO_SIGUIENTE = FUNC_ABRIENDO();
        }
        else if (ESTADO_SIGUIENTE == ESTADO_CERRANDO)
        {
            ESTADO_SIGUIENTE = FUNC_CERRANDO();
        }
        else if (ESTADO_SIGUIENTE == ESTADO_ERROR)
        {
            ESTADO_SIGUIENTE = FUNC_ERROR();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Evita bloquear CPU
    }
}
