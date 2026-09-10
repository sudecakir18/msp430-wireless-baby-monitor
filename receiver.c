#include <msp430.h>

#define BUZZER_PIN BIT0 // Buzzer connected to P1.0

void uart_init(void);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Disable Watchdog Timer
    BCSCTL1 = CALBC1_1MHZ;      // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= BUZZER_PIN;        // Set P1.0 as output
    P1OUT &= ~BUZZER_PIN;       // Buzzer initially OFF (LOW)

    uart_init();                // Initialize UART communication

    while (1)
    {
        // Check if data is received via UART
        if (IFG2 & UCA0RXIFG)
        {
            char c = UCA0RXBUF; // Read received character
            if (c == '1')
            {
                P1OUT |= BUZZER_PIN;  // Turn ON buzzer if '1' received
            }
            else if (c == '0')
            {
                P1OUT &= ~BUZZER_PIN; // Turn OFF buzzer if '0' received
            }
        }
    }
}

void uart_init(void)
{
    P1SEL |= BIT1 + BIT2;       // Assign P1.1 (RX) and P1.2 (TX) to UART
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 = UCSSEL_2;        // Select SMCLK (1MHz)
    UCA0BR0 = 104;              // 9600 baud rate @ 1MHz (1MHz / 104 ≈ 9600)
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;          // Modulation control
    UCA0CTL1 &= ~UCSWRST;       // Enable UART module
}
