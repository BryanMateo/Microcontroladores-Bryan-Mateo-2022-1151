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

uint8_t sppButtonPressed = 0;
//*********logica de GPIO***********

//*********Maquina de estado***********
uint8_t estadoActual = 0;
uint8_t estadoAnterior = 99; //valor para que se inicialice la comunicacion serial, luego se va a igualar a estado actual

//*********Maquina de estado***********

void inicializarGPIO(void);

// Función que ejecutará la tarea periódica
void maquina_estado(void *pvParameters)
{
#define TAG "Maquina de Estado"

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
        vTaskDelay(pdMS_TO_TICKS(100)); // Convierte milisegundos a ticks
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
        vTaskDelay(pdMS_TO_TICKS(100)); // Convierte milisegundos a ticks
    }
}
void app_main()
{
    // funcion de inicializacion de GPIO
    inicializarGPIO();

    // Crear la tarea Maquina de estado
    xTaskCreate(
        maquina_estado,
        "MaquinaDeEstado",
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

    ESP_LOGI(TAG, "GPIO Inicializado");
}