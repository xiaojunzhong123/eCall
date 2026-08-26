/*******************************************************
  * Name    :InputService.h
  * Author  :LoH
  * Function:Debounced digital input service interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __INPUT_SERVICE_H__
#define __INPUT_SERVICE_H__

/* Initialize input service state and RTE switch value. */
void InputService_Init(void);
/* Sample and debounce input signals at a 10 ms period. */
void InputService_Process10ms(void);

#endif
