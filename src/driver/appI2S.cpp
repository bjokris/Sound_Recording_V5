
#include <driver/i2s.h>


void appI2S_init(void)
{
    const i2s_config_t i2s_config = {
		.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
		.sample_rate =  CONFIG_MIC_SAMPLE_RATE,
		.bits_per_sample = i2s_bits_per_sample_t(CONFIG_MIC_SAMPLE_BIT),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		.communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S | I2S_COMM_FORMAT_STAND_MSB),
		.intr_alloc_flags = 0,
		.dma_buf_count = 16,
		.dma_buf_len = 512,
		.use_apll = 1,
	};

	const i2s_pin_config_t pin_config = {
		.ws_io_num = CONFIG_MIC_SCK_PIN,
		.data_in_num = CONFIG_MIC_DATA_PIN,
		// .bck_io_num = pin, => only for I2S mode
		// .data_out_num = pin,
	};

	//install and start i2s driver
	i2s_driver_install(CONFIG_MIC_PORT, &i2s_config, 0, NULL);
	i2s_set_pin(CONFIG_MIC_PORT, &pin_config);
}


void appI2S_deinit(void)
{
	i2s_zero_dma_buffer(CONFIG_MIC_PORT);
	i2s_stop(CONFIG_MIC_PORT);
	i2s_driver_uninstall(CONFIG_MIC_PORT);
}


bool appI2S_start(void)
{
	return (i2s_start(CONFIG_MIC_PORT) == ESP_OK);
}


void appI2S_stop(void)
{
	i2s_zero_dma_buffer(CONFIG_MIC_PORT);
	i2s_stop(CONFIG_MIC_PORT);
}


void appI2S_clear_buff(void)
{
	i2s_zero_dma_buffer(CONFIG_MIC_PORT);
}


int appI2S_read_data(void *data, uint16_t read_len)
{
	size_t bytes_read = 0;
	if (i2s_read(CONFIG_MIC_PORT, data, read_len, &bytes_read, portMAX_DELAY) == ESP_OK) {
		return bytes_read;
	}
	return 0;
}