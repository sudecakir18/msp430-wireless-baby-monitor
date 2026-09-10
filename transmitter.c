#include <msp430.h>

#define LED_PIN         BIT6
#define MIC_PIN         INCH_5
#define THRESHOLD       750         // ADC eşiği (ayarlanabilir)
#define LED_TIMEOUT     30          // 3 saniye (30 x 100ms)

void uart_init(void);
void uart_send_char(char c);
void adc_init(void);
unsigned int read_adc(void);
void delay_ms(unsigned int ms);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR |= LED_PIN;
    P1OUT &= ~LED_PIN;

    adc_init();
    uart_init();

    unsigned char led_timer = 0;
    char led_on = 0;

    while (1)
    {
        unsigned int mic_val = read_adc();

        if (mic_val > THRESHOLD) {
            led_timer = LED_TIMEOUT;  // Süreyi sıfırla
            if (!led_on) {
                P1OUT |= LED_PIN;
                uart_send_char('1');  // SES ALGILANDI
                led_on = 1;
            }
        }

        if (led_on) {
            if (led_timer > 0) {
                led_timer--;
            } else {
                P1OUT &= ~LED_PIN;
                uart_send_char('0');  // SES KESİLDİ
                led_on = 0;
            }
        }

        delay_ms(100);  // Her 100ms döngü
    }
}

void adc_init(void)
{
    ADC10CTL1 = MIC_PIN;
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;
}

unsigned int read_adc(void)
{
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & ADC10BUSY);
    return ADC10MEM;
}

void uart_init(void)
{
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;
}

void uart_send_char(char c)
{
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
}

void delay_ms(unsigned int ms)
{
    while (ms--) {
        __delay_cycles(1000);  // 1ms @1MHz
    }
}
