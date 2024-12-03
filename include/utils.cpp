#include "utils.h"

using namespace std;

// 将 UTF-8 编码的字符串转换为宽字符字符串
wstring utf8_to_wstring(const string& str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// 将宽字符字符串转换为 GBK 编码的字符串
string wstring_to_gbk(const wstring& wstr) {
    // 使用 locale 和 codecvt 进行转换
    locale loc("zh_CN.GBK");
    const wchar_t* wstr_data = wstr.data();
    size_t wstr_size = wstr.size();
    mbstate_t state = mbstate_t();
    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    const wchar_t* wstr_end;
    char* buffer_end;
    codecvt<wchar_t, char, mbstate_t>::result result;

    string gbk_str;
    while (wstr_size > 0) {
        result = use_facet<codecvt<wchar_t, char, mbstate_t>>(loc).out(state, wstr_data, wstr_data + wstr_size, wstr_end, buffer, buffer + buffer_size, buffer_end);
        if (result == codecvt<wchar_t, char, mbstate_t>::ok || result == codecvt<wchar_t, char, mbstate_t>::partial) {
            gbk_str.append(buffer, buffer_end - buffer);
            wstr_size -= wstr_end - wstr_data;
            wstr_data = wstr_end;
        } else {
            cerr << "Error converting to GBK" << endl;
            break;
        }
    }
    return gbk_str;
}

// 将八进制GBK编码的字符串转换为十六进制字符串数组
vector<uint16_t> gbk_hex(const string& s){
    vector<uint16_t> hex_str16;
    wstring wide_str = utf8_to_wstring(s);

    string gbk_str = wstring_to_gbk(wide_str);
    // 将八进制转换十六进制，(unsigned char)消除了多余的ffff
    for(int i = 0, length_s = gbk_str.size(); i < length_s; i += 2) {
        uint8_t h = (uint8_t)(unsigned char)gbk_str[i]; 
        uint8_t l = (uint8_t)(unsigned char)gbk_str[i + 1];
        hex_str16.push_back((h << 8) + l);
    }
    
    return hex_str16;
}