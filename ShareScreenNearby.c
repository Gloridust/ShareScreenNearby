#include <windows.h>
#include <winsock2.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdint.h>
#include <jpeglib.h>
#include <io.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")

ULONG_PTR gdiplusToken;

void save_jpeg(const char *filename, HBITMAP hBitmap) {
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = -bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    BYTE *pPixels = (BYTE*)malloc(bmp.bmWidth * 3 * bmp.bmHeight);
    GetDIBits(GetDC(0), hBitmap, 0, bmp.bmHeight, pPixels, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile = fopen(filename, "wb");
    if (!outfile) {
        free(pPixels);
        return;
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = bmp.bmWidth;
    cinfo.image_height = bmp.bmHeight;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer[1];
    int row_stride = bmp.bmWidth * 3;
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &pPixels[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    free(pPixels);
}

void capture_screenshot(const char *filename) {
    HDC hdc = GetDC(NULL);
    HDC hDest = CreateCompatibleDC(hdc);
    RECT rc;
    GetClientRect(GetDesktopWindow(), &rc);
    HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    SelectObject(hDest, hbDesktop);
    BitBlt(hDest, 0, 0, rc.right, rc.bottom, hdc, 0, 0, SRCCOPY);
    save_jpeg(filename, hbDesktop);
    DeleteObject(hbDesktop);
    DeleteDC(hDest);
    ReleaseDC(NULL, hdc);
}

void handle_client(SOCKET client_socket) {
    const char header[] = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
                          "\r\n";
    send(client_socket, header, sizeof(header) - 1, 0);

    while (1) {
        capture_screenshot("screenshot.jpg");

        FILE *file = fopen("screenshot.jpg", "rb");
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *file_buffer = (char*)malloc(file_size);
        fread(file_buffer, 1, file_size, file);
        fclose(file);

        char frame_header[100];
        int frame_header_len = sprintf(frame_header, "--frame\r\n"
                                                     "Content-Type: image/jpeg\r\n"
                                                     "Content-Length: %ld\r\n\r\n", file_size);
        send(client_socket, frame_header, frame_header_len, 0);
        send(client_socket, file_buffer, file_size, 0);
        free(file_buffer);

        Sleep(100);
    }
}

int main() {
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Initialize Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    printf("请输入端口号：");
    int port;
    scanf("%d", &port);
    server_addr.sin_port = htons(port);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("Server listening on port %d\n", port);

    // Handle client connections
    while (1) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket != INVALID_SOCKET) {
            handle_client(client_socket);
            closesocket(client_socket);
        }
    }

    // Cleanup
    closesocket(server_socket);
    WSACleanup();
    GdiplusShutdown(gdiplusToken);

    return 0;
}
