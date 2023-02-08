#include "usart.h"

int _write(int file, void *ptr, size_t len)
{
	// Transmit printf string to USART - blocking call
	HAL_UART_Transmit(&huart3, (uint8_t *) ptr, len, 10000);
	return len;
}
_ssize_t _write_r (struct _reent *ptr, int fd, const void *buf, size_t cnt)
{
	return _write(fd, (char *)buf, cnt);
}

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t cnt)
{
	HAL_StatusTypeDef ret = HAL_BUSY;
	while (ret != HAL_OK) {
		ret = HAL_UART_Receive(&huart3, buf, 1, 10000);
	}
	return 1;
}
