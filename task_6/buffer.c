#include "header.h"

static size_t b_size = 0;


void SystemInit(size_t buffer_size) {
	b_size = buffer_size;
}


void SystemClose() {
	b_size = 0;
}


bufs* open(const char* name, const char mode) {
	bufs* bf = malloc(sizeof(bufs));

	if (!bf) {
		return NULL;
	}


	bf->buf = malloc(b_size);
	if (!bf->buf) {
		free(bf);
		return NULL;
	}


	if (mode == 'r') {
		bf->file = fopen(name, "r");
	}
	else if (mode == 'w') {
		bf->file = fopen(name, "w");
	}
	else {
		free(bf->buf);
		free(bf);
		return NULL;
	}

	if (!bf->file) {
		free(bf->buf);
		free(bf);
		return NULL;
	}

	bf->buf_size = b_size;
	bf->buf_pos = 0;
	bf->mode = mode;

	return bf;
}

void close(bufs* bf) {
	if (!bf) return;

	if (bf->mode == 'w' && bf->buf_pos > 0) {
		fwrite(bf->buf, 1, bf->buf_pos, bf->file);
	}

	fclose(bf->file);
	free(bf->buf);
	free(bf);
}


size_t read(bufs* bf, void* dest, size_t to_read) {

	if (bf->mode != 'r') {
		return 0;
	}

	size_t bytes_read = 0;
	char* storage = dest;

	while (to_read > 0) {
		if (bf->buf_pos == 0 || bf->buf_pos >= bf->buf_size) {
			bf->buf_size = fread(bf->buf, 1, b_size, bf->file);
			bf->buf_pos = 0;
			if (bf->buf_size == 0) {
				break;
			}
		}

		size_t to_copy = bf->buf_size - bf->buf_pos;

		if (to_copy > to_read) {
			to_copy = to_read;
		}

		memcpy(storage, bf->buf + bf->buf_pos, to_copy);

		bf->buf_pos += to_copy;
		storage += to_copy;
		to_read -= to_copy;
		bytes_read += to_copy;
	}


	return bytes_read;
}


size_t write(bufs* bf, const void* src, size_t to_write) {
	if (bf->mode != 'w') {
		return 0;
	}

	size_t bytes_written = 0;
	const char* storage = (const char*)src;


	while (to_write > 0) {
		size_t space_left = b_size - bf->buf_pos;

		if (space_left == 0) {
			fwrite(bf->buf, 1, b_size, bf->file);
			bf->buf_pos = 0;
			space_left = b_size;
		}


		size_t to_copy;
		if (to_write < space_left) {
			to_copy = to_write;
		}
		else {
			to_copy = space_left;
		}


		memcpy(bf->buf + bf->buf_pos, storage, to_copy);



		bf->buf_pos += to_copy;
		storage += to_copy;
		to_write -= to_copy;
		bytes_written += to_copy;
	}

	return bytes_written;
}


