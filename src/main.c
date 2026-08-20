
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <math.h>

#define PWM_NODE DT_NODELABEL(pwm_led0)
static const struct pwm_dt_spec pwm_led =PWM_DT_SPEC_GET(PWM_NODE);

//#define HR_FREQ_HZ 0.5f //0,5 Hz->30 BPM
//#define HR_FREQ_HZ 1.0f //1 Hz->60 BPM
//#define HR_FREQ_HZ 1.5f //1,5 Hz->90 BPM
//#define HR_FREQ_HZ 2.0f //2 Hz->120 BPM
//#define HR_FREQ_HZ 2.5f //2,5 Hz->150 BPM
//#define HR_FREQ_HZ 3.0f //3 Hz->180 BPM
#define HR_FREQ_HZ 3.5f //3,5 Hz->210 BPM
#define PWM_PERIOD_NS 10000000 //=10ms=0.01s -> f=1/0.01=100Hz
#define M_PI 3.14159265358979323846f

int main(void)
{
	int ret;
	float phase;
	uint32_t time_ms;
	uint32_t duty_cycle_ns;

	if(!pwm_is_ready_dt(&pwm_led)){
		printk("PWM not ready!\n");
		return 0;
	}

	ret=pwm_set_dt(&pwm_led,PWM_PERIOD_NS,0);
	if(ret<0){
		printk("PWM init failed: %d\n",ret);
		return 0;
	}
	printk("HR simulator-%d Hz (%d BPM)\n",(int)HR_FREQ_HZ,(int)HR_FREQ_HZ*60);

	while (1) {
		time_ms=k_uptime_get_32();
		phase=2.0f*M_PI*HR_FREQ_HZ*(time_ms/1000.0f);
		float duty=(sinf(phase)+1.0f)/2.0f; //[0,1]
		duty_cycle_ns=PWM_PERIOD_NS*duty; 
		ret=pwm_set_pulse_dt(&pwm_led, duty_cycle_ns);
		if(ret<0){
			printk("PWM set failed: %d\n",ret);
		}

		k_sleep(K_MSEC(1));
	}
	return 0;
}
