#pragma once

class BufferedReader {
 private:
     PackageStream* ptr;
     char* temp_buf;
     char* extra_buf;
     int32_t extra_count;
 public:
    explicit BufferedReader(PackageStream* stream) :
        ptr(stream),
        temp_buf(new char[ptr->PackageLen()]),
        extra_buf(new char[ptr->PackageLen()]),
        extra_count(0) {}
    ~BufferedReader() {
        delete[] temp_buf;
        delete[] extra_buf;
    }
    int32_t Read(char* output_buffer, int32_t buffer_len) {
        int32_t total_len = 0, package = 0;
        bool read = true;
        while (read) {
            if (extra_count) {
                package = extra_count;
                memcpy(temp_buf, extra_buf, extra_count);
                extra_count = 0;
            } else {
                package = ptr->ReadPackage(temp_buf);
            }
            if (total_len + package <= buffer_len && package > 0) {
                memcpy(output_buffer + total_len, temp_buf, package);
                total_len += package;
            } else if (total_len + package > buffer_len && package > 0) {
                memcpy(output_buffer + total_len, temp_buf,
                    buffer_len - total_len);
                extra_count = package - (buffer_len - total_len);
                memcpy(extra_buf, temp_buf + (buffer_len - total_len),
                    extra_count);
                total_len += buffer_len - total_len;
                read = false;
            } else {
                read = false;
            }
        }
        return total_len;
    }
};
