
#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4} channel_t;

void ADC_init(void);

char ADC_get_data(void);

void ADC_read(channel_t channel);


#endif /* ADC_DRIVER_H_ */
