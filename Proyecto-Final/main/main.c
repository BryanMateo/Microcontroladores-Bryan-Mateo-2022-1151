#include <stdio.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <string.h>

//*********logica de GPIO***********
#define logica "Negativa"
// #define logica "Positiva"

uint8_t TRUE;
uint8_t FALSE;

#define sppButton GPIO_NUM_23
#define led0 GPIO_NUM_2

uint8_t sppButtonPressed = 0;
//*********logica de GPIO***********

//*********Maquina de estado***********
uint8_t estadoActual = 0;
uint8_t estadoAnterior = 99; // valor para que se inicialice la comunicacion serial, luego se va a igualar a estado actual

//*********Maquina de estado***********

//*********Timer**************

//*********Timer**************

void inicializarGPIO(void);

// Función que ejecutará la tarea periódica
void maquina_estado(void *pvParameters)
{
    while (1)
    {
        if (sppButtonPressed == 0) // condicion para solo leer un pulso
        {
            if (gpio_get_level(sppButton) == TRUE)
            {
                sppButtonPressed = 1;
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

void info_serial(void *pvParameters)
{
#define TAG "Info Serial"

    while (1)
    {
        if (estadoAnterior != estadoActual)
        {
            ESP_LOGI(TAG, "Estado = %d", estadoActual);
            estadoAnterior = estadoActual;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void led_blink(void *pvParameters)
{

    uint16_t contadorActual = 0;
    uint8_t ledLevel = 0;
    uint8_t estadoLedAnterior = 0; // variable para reiniciar los contadores de tiempo
    uint8_t periodoBarrido = 0;
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
    // funcion de inicializacion de GPIO
    inicializarGPIO();

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

    ESP_LOGD(TAG, "Tarea creada. El sistema está funcionando.");
}

void inicializarGPIO()
{
#define TAG "GPIO"
    if (strcmp(logica, "Negativa") == 0)
    {
        TRUE = 0;
        FALSE = 1;
        ESP_LOGI(TAG, "Logica Negativa");
    }
    else if (strcmp(logica, "Positiva") == 0)
    {
        TRUE = 1;
        FALSE = 0;
        ESP_LOGI(TAG, "Logica Positiva");
    }
    else
        ESP_LOGE(TAG, "Error Logica Incorrecta");

    // Boton SPP Logica Negativa
    gpio_reset_pin(sppButton);
    gpio_set_direction(sppButton, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sppButton, GPIO_PULLUP_ONLY);

    // Led integrado ESP32
    gpio_reset_pin(led0);
    gpio_set_direction(led0, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "GPIO Inicializado");
}