/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MAIN_H    /* Guard against multiple inclusion */
#define _MAIN_H
    void blinkTimer(TC_TIMER_STATUS event,uintptr_t context );
    void sampleTimer(TC_TIMER_STATUS event,uintptr_t context );
    void mySercom3(SERCOM_USART_EVENT event, uintptr_t context );
    void mySercom5(SERCOM_USART_EVENT event, uintptr_t context );
    void InitRN(void);
    void InitSystem(void);
#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
