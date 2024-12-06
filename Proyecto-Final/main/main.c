#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <string.h>

#define logica "Negativa"
// #define logica "Positiva"
uint8_t TRUE;
uint8_t FALSE;

#define sppButton GPIO_NUM_21

void inicializarGPIO(void);

// Función que ejecutará la tarea periódica
void maquina_estado(void *pvParameters)
{
#define TAG "Maquina de Estado"

    while (1)
    {
        if (gpio_get_level(sppButton) == 0)
        {
            ESP_LOGI(TAG, "SPP Presionado");
        }
        
        // Delay de 100 ms
        vTaskDelay(pdMS_TO_TICKS(100)); // Convierte milisegundos a ticks
    }
}

void app_main()
{
    // funcion de inicializacion de GPIO
    inicializarGPIO();

    // Crear la tarea Maquina de estado
    xTaskCreate(
        maquina_estado,    // Función de la tarea
        "MaquinaDeEstado", // Nombre de la tarea
        2048,              // Tamaño de la pila en palabras
        NULL,              // Parámetro de la tarea
        5,                 // Prioridad de la tarea
        NULL               // Handle de la tarea (opcional)
    );

    ESP_LOGI(TAG, "Tarea creada. El sistema está funcionando.");
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