#define Il2CppPath OBFUSCATE("lib/arm64/libil2cpp.so")
#define SharedPath OBFUSCATE("lib/arm64/libshared.so")
#define UnityPath OBFUSCATE("lib/arm64/libunity.so")

bool contains(std::string in, std::string target) {
    if (strstr(in.c_str(), target.c_str())) {
        return true;
    }
    return false;
}

bool equals(std::string first, std::string second) {
    if (first == second) {
        return true;
    }
    return false;
}

std::string utf16le_to_utf8(const std::u16string &u16str) {
    if (u16str.empty()) { return std::string(); }
    const char16_t *p = u16str.data();
    std::u16string::size_type len = u16str.length();
    if (p[0] == 0xFEFF) {
        p += 1;
        len -= 1;
    }
    std::string u8str;
    u8str.reserve(len * 3);
    char16_t u16char;
    for (std::u16string::size_type i = 0; i < len; ++i) {
        u16char = p[i];
        if (u16char < 0x0080) {
            u8str.push_back((char) (u16char & 0x00FF));
            continue;
        }
        if (u16char >= 0x0080 && u16char <= 0x07FF) {
            u8str.push_back((char) (((u16char >> 6) & 0x1F) | 0xC0));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
        if (u16char >= 0xD800 && u16char <= 0xDBFF) {
            uint32_t highSur = u16char;
            uint32_t lowSur = p[++i];
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            u8str.push_back((char) ((codePoint >> 18) | 0xF0));
            u8str.push_back((char) (((codePoint >> 12) & 0x3F) | 0x80));
            u8str.push_back((char) (((codePoint >> 06) & 0x3F) | 0x80));
            u8str.push_back((char) ((codePoint & 0x3F) | 0x80));
            continue;
        }
        {
            u8str.push_back((char) (((u16char >> 12) & 0x0F) | 0xE0));
            u8str.push_back((char) (((u16char >> 6) & 0x3F) | 0x80));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
    }

    return u8str;
}

typedef struct _monoString {
    void *klass;
    void *monitor;
    int length;
    const char *toChars(){
        std::u16string ss((char16_t *) getChars(), 0, getLength());
        std::string str = utf16le_to_utf8(ss);
        return str.c_str();
    }
    char chars[0];

    char *getChars() {
        return chars;
    }
    std::string str() {
        return std::string(getChars());
    }
    int getLength() {
        return length;
    }
} monoString;

class Cipher {
    const std::unordered_map<char, char> map;
public:
    explicit Cipher(int n);
    std::string encrypt(std::string) const;
private:
    static std::unordered_map<char, char> make_map(int n);
};

Cipher::Cipher(int n) : map{make_map(n)} {}

std::unordered_map<char, char> Cipher::make_map(int n) {
    // helper function to give a positive value for a%b
    auto mod = [](int a, int b){ a %= b; return a < 0 ? a + b : a; };
    std::unordered_map<char, char> map; {
        static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int const m = sizeof alphabet - 1; // subtract the final NUL
        for (int i = 0;  i < m;  ++i) {
            map.insert(std::make_pair(alphabet[i], alphabet[mod(i+n,m)]));
        }
    }
	static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
	int const m = sizeof alphabet - 1; // subtract the final NUL
	for (int i = 0; i < m; i++) {
		map.insert(std::make_pair(alphabet[i], alphabet[mod(i+n,m)]));
	}
    return map;
}

std::string Cipher::encrypt(std::string s) const {
    std::transform(s.begin(), s.end(), s.begin(), [=](char c){ auto it = map.find(c); return it == map.end() ? c : it->second; });
    return s;
}

std::string decit(std::string targ) {
	Cipher cipher(atoi(OBFUSCATE("-2077")));
	return cipher.encrypt(targ);
}

//std::string g_key = "zxcvbnmlkjhgfdsa";
//std::string g_iv = "asdfghjklzxcvbnm";

std::string AESEncrypt(const std::string& strSrc){
    size_t length = strSrc.length();
    int block_num = length / BLOCK_SIZE + 1;
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc.c_str());
    int k = length % BLOCK_SIZE;
    int j = length / BLOCK_SIZE;
    int padding = BLOCK_SIZE - k;
    for (int i = 0; i < padding; i++) {
        szDataIn[j * BLOCK_SIZE + k + i] = padding;
    }
    szDataIn[block_num * BLOCK_SIZE] = '\0';
    char *szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);
    AES aes;
    aes.MakeKey(decit(OBFUSCATE("wuzsykjihgedcapx")).c_str(), decit(OBFUSCATE("xpacdeghiwuzsykj")).c_str(), 16, 16);
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    std::string str = base64_encode((unsigned char*) szDataOut, block_num * BLOCK_SIZE);
    delete[] szDataIn;
    delete[] szDataOut;
    return str;
}

std::string AESDecrypt(const std::string& strSrc) {
    std::string strData = base64_decode(strSrc);
    size_t length = strData.length();
    char *szDataIn = new char[length + 1];
    memcpy(szDataIn, strData.c_str(), length+1);
    char *szDataOut = new char[length + 1];
    memcpy(szDataOut, strData.c_str(), length+1);
    AES aes;
    aes.MakeKey(decit(OBFUSCATE("wuzsykjihgedcapx")).c_str(), decit(OBFUSCATE("xpacdeghiwuzsykj")).c_str(), 16, 16);
    aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);
    if (0x00 < szDataOut[length - 1] <= 0x16) {
        int tmp = szDataOut[length - 1];
        for (int i = length - 1; i >= length - tmp; i--) {
            if (szDataOut[i] != tmp) {
                memset(szDataOut, 0, length);
                break;
            }
            else
                szDataOut[i] = 0;
        }
    }
    std::string strDest(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return strDest;
}

std::string xor_cipher(const std::string &data, const std::string &key, bool mode) {
    std::string result = data;
    uint32_t key1 = 0x1EFF2FE1, key2 = 0x1E00A2E3;
    for (char c : key) {
        key1 = (key1 * 33) ^ static_cast<uint8_t>(c);
        key2 = (key2 * 31) + static_cast<uint8_t>(c);
    }
    for (size_t i = 0; i < result.size(); ++i) {
        if (mode) { // Encrypt
            result[i] = (result[i] << 3) | (result[i] >> 5);
            result[i] ^= static_cast<uint8_t>(key1 >> (i % 32));
            result[i] = (result[i] >> 2) | (result[i] << 6);
            result[i] ^= static_cast<uint8_t>(key2 >> ((i + 5) % 32));
        } else { // Decrypt
            result[i] ^= static_cast<uint8_t>(key2 >> ((i + 5) % 32));
            result[i] = (result[i] << 2) | (result[i] >> 6);
            result[i] ^= static_cast<uint8_t>(key1 >> (i % 32));
            result[i] = (result[i] >> 3) | (result[i] << 5);
        }
    }
    return result;
}

bool isLibraryLoaded(const char *libraryName) {
    //libLoaded = true;
    char line[512] = {0};
    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            std::string a = line;
            if (!strstr(line, OBFUSCATE("r--p")) && strstr(line, libraryName)) {
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

std::string RPB(std::string str) {
	return ReplaceString(ReplaceString(str, "\"", ""), "\"", "");
}

void *il2cpp_handle() {
	return xdl_open("libil2cpp.so", XDL_DEFAULT);
}

#define DO_API(r, n, p) r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

void init_api() {
	#define DO_API(r, n, p) n = (r (*) p)xdl_sym(il2cpp_handle(), #n, NULL)
	#include "il2cpp-api-functions.h"
	#undef DO_API
}

/*FieldInfo* GetFieldByName(const char* namespaze, const char* clazz, const char* fname) {
	return il2cpp_class_get_field_from_name(il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), OBFUSCATE("Assembly-CSharp"))), namespaze, clazz), fname);
}

FieldInfo* GetFieldFromClass(Il2CppClass* clazz, const char* fname) {
	return il2cpp_class_get_field_from_name(clazz, fname);
}

const MethodInfo* GetMethodFromName(const char* namespaze, const char* clazz, const char* mname, int argsCount = 0) {
	return il2cpp_class_get_method_from_name(il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), OBFUSCATE("Assembly-CSharp"))), namespaze, clazz), mname, argsCount);
}

const MethodInfo* GetMethodFromNameUnity(const char* namespaze, const char* clazz, const char* mname, int argsCount = 0) {
	return il2cpp_class_get_method_from_name(il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), OBFUSCATE("UnityEngine"))), namespaze, clazz), mname, argsCount);
}

const MethodInfo* GetMethodFromA(const char* assem, const char* namespaze, const char* clazz, const char* mname, int argsCount = 0) {
	return il2cpp_class_get_method_from_name(il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), assem)), namespaze, clazz), mname, argsCount);
}

Il2CppClass* GetClassFromName(const char* namespaze, const char* clazz) {
	return il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), OBFUSCATE("Assembly-CSharp"))), namespaze, clazz);
}

Il2CppClass* GetClassFromA(const char* assem, const char* namespaze, const char* clazz) {
	return il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(il2cpp_domain_get(), assem)), namespaze, clazz);
}

const MethodInfo* GetMethodFromClass(Il2CppClass* clazz, const char* mname, int argsCount = 0) {
	return il2cpp_class_get_method_from_name(clazz, mname, argsCount);
}*/

monoString* CreateMonoString(const char* str) {
	return (monoString*) il2cpp_string_new(str);
}

static int callback_z(struct dl_phdr_info *info, size_t size, void *arg) {
  (void)size, (void)arg;
  if (contains(info->dlpi_name, Il2CppPath)) {
	  il2cpp_base = (uintptr_t)info->dlpi_addr;
  } else if (contains(info->dlpi_name, UnityPath)) {
	  unity_base = (uintptr_t)info->dlpi_addr;
      il2cpp_base = (uintptr_t)info->dlpi_addr;
  } else if (contains(info->dlpi_name, SharedPath)) {
	  shared_base = (uintptr_t)info->dlpi_addr;
  } else if (contains(info->dlpi_name, "libEGL.so")) {
	  egl_base = (uintptr_t)info->dlpi_addr;
  }
  return 0;
}

/*
static int callback_z(struct dl_phdr_info *info, size_t size, void *arg) {
    (void)size, (void)arg;
    
    uintptr_t *base = NULL;
    size_t *length = NULL;
    
    if (contains(info->dlpi_name, Il2CppPath)) {
        base = &il2cpp_base;
        length = &il2cpp_length;
    } else if (contains(info->dlpi_name, UnityPath)) {
        base = &unity_base;
        length = &unity_length;
    } else if (contains(info->dlpi_name, SharedPath)) {
        base = &shared_base;
        length = &shared_length;
    } else if (contains(info->dlpi_name, "libEGL.so")) {
        base = &egl_base;
        length = &egl_length;
    }

    if (!base || !length) {
        *base = (uintptr_t)info->dlpi_addr;
        size_t max_offset = 0;

        for (int i = 0; i < info->dlpi_phnum; i++) {
            size_t end = info->dlpi_phdr[i].p_vaddr + info->dlpi_phdr[i].p_memsz;
            if (end > max_offset) {
                max_offset = end;
            }
        }

        *length = max_offset;
    }

    return 0;
}*/

int getPID(const char *PackageName) {
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    FILE *fp = NULL;
    char filepath[256];
    char filetext[128];

    dir = opendir(OBFUSCATE("/proc"));
    if (dir != NULL) {
        while ((ptr = readdir(dir)) != NULL) {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, OBFUSCATE("..")) == 0)) continue;
            if (ptr->d_type != DT_DIR) continue;

            sprintf(filepath, OBFUSCATE("/proc/%s/cmdline"), ptr->d_name);
            fp = fopen(filepath, OBFUSCATE("r"));
            if (fp != NULL) {
                fgets(filetext, sizeof(filetext), fp);
                if (strcmp(filetext, PackageName) == 0) break;
                fclose(fp);
            }
        }
    }
    if (readdir(dir) == NULL) return 0;

    closedir(dir);
    return atoi(ptr->d_name);
}

int killprocess(const char *bm) {
    int pid = getPID(bm);
    if (pid == 0) return -1;

    char ml[32];
    sprintf(ml, OBFUSCATE("kill %d"), pid);
    system(ml);

    return 0;
}

int killGG() {
    DIR *dir = NULL;
    DIR *dirGG = NULL;
    struct dirent *ptr = NULL;
    struct dirent *ptrGG = NULL;
    char filepath[256];
    int flag = 1;

    dir = opendir(OBFUSCATE("/data/data"));
    if (dir != NULL) {
        while (flag && (ptr = readdir(dir)) != NULL) {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, OBFUSCATE("..")) == 0)) continue;
            if (ptr->d_type != DT_DIR) continue;

            sprintf(filepath, OBFUSCATE("/data/data/%s/files"), ptr->d_name);
            dirGG = opendir(filepath);
            if (dirGG != NULL) {
                while ((ptrGG = readdir(dirGG)) != NULL) {
                    if ((strcmp(ptrGG->d_name, ".") == 0) || (strcmp(ptr->d_name, OBFUSCATE("..")) == 0)) continue;
                    if (ptrGG->d_type != DT_DIR) continue;

                    if (strstr(ptrGG->d_name, OBFUSCATE("GG"))) {
                        int pid = getPID(ptr->d_name);
                        if (pid == 0) continue;
                        else killprocess(ptr->d_name);
                    }
                }
            }
        }
    }
    closedir(dir);
    closedir(dirGG);
    return 0;
}

int generate_num(const int len) {
    string id;
    static const char gsfid[] = "1234567890";
    srand((unsigned) time(nullptr) * getpid());
    id.reserve(len);
    for (int i = 0; i < len; ++i) id += gsfid[rand() % (sizeof(gsfid) - 1)];
    return std::atoi(id.c_str());
}

std::string string_to_hex(const std::string& input) {
    static const char hex_digits[] = "0123456789ABCDEF";
    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

int hex_value(unsigned char hex_digit) {
    static const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    int value = hex_values[hex_digit];
    if (value == -1) throw std::invalid_argument(std::string(OBFUSCATE("invalid hex digit")));
    return value;
}

std::string hex_to_string(const std::string& input) {
    const auto len = input.length();
    if (len & 1) throw std::invalid_argument(std::string(OBFUSCATE("odd length")));
    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end(); ) {
        int hi = hex_value(*it++);
        int lo = hex_value(*it++);
        output.push_back(hi << 4 | lo);
    }
    return output;
}

typedef unsigned long DWORD;

int libii = 0, libuu = 0;

DWORD findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;

    sprintf(filename, OBFUSCATE("/proc/self/maps"));

    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }
    
   /* if (strstr(library, "libil2cpp.so") || strstr(library, "libunity.so")) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, OBFUSCATE("r--p")) && strstr(buffer, library)) {
                address = (DWORD) strtoul(buffer, NULL, 16);
                if (strstr(library, "libil2cpp.so")) {
                    if (libii = 1) {
                        goto done;
                    }
                    libii += 1;
                } else if (strstr(library, "libunity.so")) {
                    if (libuu = 1) {
                        goto done;
                    }
                    libuu += 1;
                }
                //goto done;
            }
        }
    } else {*/
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, library)) {
                address = (DWORD) strtoul(buffer, NULL, 16);
                goto done;
            }
        }
    //}
    

    done:

    if (fp) {
        fclose(fp);
    }

    return address;
}

class Rect {
public:
	float x;
	float y;
	float width;
	float height;

	Rect() {
		this->x = 0;
		this->y = 0;
		this->width = 0;
		this->height = 0;
	}

	Rect(float x, float y, float width, float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	bool operator==(const Rect &src) const {
		return (src.x == this->x && src.y == this->y && src.height == this->height &&
				src.width == this->width);
	}

	bool operator!=(const Rect &src) const {
		return (src.x != this->x && src.y != this->y && src.height != this->height &&
				src.width != this->width);
	}
};

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr) {
    uintptr_t libBase = 0x0;
    if (strstr(libraryName, OBFUSCATE("libil2cpp.so"))) {
        uintptr_t libBase = il2cpp_base;
    } else {
        uintptr_t libBase = findLibrary(libraryName);
    }
    if (!libBase) return 0;
    return (reinterpret_cast<DWORD>(libBase + relativeAddr));
}


jobject GetActivityContext(JNIEnv* env) {
    jclass uplayer = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayer"));
    jfieldID cmeth = env->GetStaticFieldID(uplayer, OBFUSCATE("currentActivity"), OBFUSCATE("Landroid/app/Activity;"));
    jobject currt = env->NewGlobalRef(env->GetStaticObjectField(uplayer, cmeth));
    return currt;
}

jobject GetContext(JNIEnv* globalEnv) {
    jclass activityThread = globalEnv->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThread = globalEnv->GetStaticMethodID(activityThread, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject at = globalEnv->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = globalEnv->GetMethodID(activityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
    jobject context = globalEnv->CallObjectMethod(at, getApplication);
    return context;
}

void Toast(JNIEnv *env, const char *text, int length) {
    jstring jstr = env->NewStringUTF(text);
    jclass toast = env->FindClass(OBFUSCATE("android/widget/Toast"));
    jmethodID methodMakeText =env->GetStaticMethodID(toast,OBFUSCATE("makeText"),OBFUSCATE("(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"));
    jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText, GetContext(env), jstr, length);
    jmethodID methodShow = env->GetMethodID(toast, OBFUSCATE("show"), OBFUSCATE("()V"));
    env->CallVoidMethod(toastobj, methodShow);
}

bool isconnected(JNIEnv* env, jobject context) {
    jclass cont = env->FindClass(OBFUSCATE("android/content/Context"));
    jclass cn = env->FindClass(OBFUSCATE("android/net/ConnectivityManager"));
    jclass netc = env->FindClass(OBFUSCATE("android/net/NetworkCapabilities"));
    jmethodID getser = env->GetMethodID(cont, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jmethodID getcncaps = env->GetMethodID(cn, OBFUSCATE("getNetworkCapabilities"), OBFUSCATE("(Landroid/net/Network;)Landroid/net/NetworkCapabilities;"));
    jmethodID getact = env->GetMethodID(cn, OBFUSCATE("getActiveNetwork"), OBFUSCATE("()Landroid/net/Network;"));
    jmethodID gethas = env->GetMethodID(netc, OBFUSCATE("hasTransport"), OBFUSCATE("(I)Z"));
    jobject conser = env->CallObjectMethod(context, getser, env->NewStringUTF(OBFUSCATE("connectivity")));
    jobject getcnact = (jobject) env->CallObjectMethod(conser, getact);
    jobject getcnc = (jobject) env->CallObjectMethod(conser, getcncaps, getcnact);
    return env->CallBooleanMethod(getcnc, gethas, std::atoi(OBFUSCATE("4")));
}

void copyText(JNIEnv* env, jobject context, std::string text) {
    jclass aycls = env->FindClass(OBFUSCATE("android/app/Activity"));
    jmethodID gss = env->GetMethodID(aycls, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
    jobject ss = env->CallObjectMethod(context, gss, env->NewStringUTF(OBFUSCATE("clipboard")));
    jclass clcls = env->FindClass(OBFUSCATE("android/content/ClipData"));
    jmethodID clmeth = env->GetStaticMethodID(clcls, OBFUSCATE("newPlainText"), OBFUSCATE("(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;"));
    jclass imcls = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
    jmethodID tgsifm = env->GetMethodID(imcls, OBFUSCATE("setPrimaryClip"), OBFUSCATE("(Landroid/content/ClipData;)V"));
    env->CallVoidMethod(ss, tgsifm, env->CallStaticObjectMethod(clcls, clmeth, env->NewStringUTF(OBFUSCATE("unknown")), env->NewStringUTF(text.c_str())));
    Toast(env, OBFUSCATE("Copied to Clipboard!"), 1);
}

const char* readClip(JNIEnv* env, jobject context) {
    jclass uplr = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayer"));
    jmethodID uobj = env->GetMethodID(uplr, OBFUSCATE("<init>"), OBFUSCATE("(Landroid/content/Context;)V"));
    jobject umain = env->NewObject(uplr, uobj, context);
    jmethodID lmao = env->GetMethodID(uplr, OBFUSCATE("getClipboardText"), OBFUSCATE("()Ljava/lang/String;"));
    jstring fvalue = static_cast<jstring>(env->CallObjectMethod(umain, lmao));
    return env->GetStringUTFChars(fvalue, 0);
}

std::string getStorage(JNIEnv* globalEnv) {
    jclass activityClass = globalEnv->FindClass(OBFUSCATE("android/os/Environment"));
    jmethodID getFilesDirMethod = globalEnv->GetStaticMethodID(activityClass, OBFUSCATE("getExternalStorageDirectory"), OBFUSCATE("()Ljava/io/File;"));
    jobject filesDirObj = globalEnv->CallStaticObjectMethod(activityClass, getFilesDirMethod);
    jclass fileClass = globalEnv->FindClass(OBFUSCATE("java/io/File"));
    jmethodID getPathMethod = globalEnv->GetMethodID(fileClass, OBFUSCATE("getAbsolutePath"), OBFUSCATE("()Ljava/lang/String;"));
    jstring cacheDir = (jstring) globalEnv->CallObjectMethod(filesDirObj, getPathMethod);
    return std::string(globalEnv->GetStringUTFChars(cacheDir, 0));
}

const char* getCacheDir(JNIEnv *env) {
  jclass activityClass = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayerActivity"));
  jmethodID getFilesDirMethod = env->GetMethodID(activityClass, OBFUSCATE("getCacheDir"), OBFUSCATE("()Ljava/io/File;"));
  jobject filesDirObj = env->CallObjectMethod(GetContext(env), getFilesDirMethod);
  jclass fileClass = env->FindClass(OBFUSCATE("java/io/File"));
  jmethodID getPathMethod = env->GetMethodID(fileClass, OBFUSCATE("getAbsolutePath"), OBFUSCATE("()Ljava/lang/String;"));
  jstring pathObj = (jstring) env->CallObjectMethod(filesDirObj, getPathMethod);
  const char* filesDir = env->GetStringUTFChars(pathObj, NULL);
  //env->ReleaseStringUTFChars(pathObj, dir);
  return filesDir;
}

const char* getApkPath(JNIEnv *globalEnv, jobject context) {
    jclass contextClass = globalEnv->GetObjectClass(context);
    jmethodID getApplicationInfo = globalEnv->GetMethodID(contextClass, OBFUSCATE("getApplicationInfo"), OBFUSCATE("()Landroid/content/pm/ApplicationInfo;"));
    jobject ApplicationInfo_obj = globalEnv->CallObjectMethod(context, getApplicationInfo);
    jclass ApplicationInfoClass = globalEnv->GetObjectClass(ApplicationInfo_obj);
    jmethodID getPackageResourcePath = globalEnv->GetMethodID(contextClass, OBFUSCATE("getPackageResourcePath"), OBFUSCATE("()Ljava/lang/String;"));
    jstring mPackageFilePath = static_cast<jstring>(globalEnv->CallObjectMethod(context, getPackageResourcePath));
    return globalEnv->GetStringUTFChars(mPackageFilePath, 0);
}

const char* getLocale(JNIEnv *env) {
    jclass activityClass = env->FindClass(OBFUSCATE("java/util/Locale"));
    jmethodID getdef = env->GetStaticMethodID(activityClass, OBFUSCATE("getDefault"), OBFUSCATE("()Ljava/util/Locale;"));
    jobject getdefobj = env->CallStaticObjectMethod(activityClass, getdef);
    jmethodID toStr = env->GetMethodID(activityClass, OBFUSCATE("toLanguageTag"), OBFUSCATE("()Ljava/lang/String;"));
    jstring loc = static_cast<jstring>(env->CallObjectMethod(getdefobj, toStr));
    return env->GetStringUTFChars(loc, 0);
}
