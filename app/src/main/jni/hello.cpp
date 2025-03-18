#include <pthread.h>
#include <dlfcn.h>
#include <jni.h>
#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <list>
#include <locale>
#include <string>
#include <stdint.h>
#include <cstring>
#include <string.h>
#include <wchar.h>
#include <endian.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <csignal>
#include <sys/system_properties.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/md5.h>

#include "AES.h"
#include "base64.h"
#include "dobby.h"
#include "FLog.h"
#include "curl/curl.h"
#include "hide.h"
#include "json.hpp"
#include "NikkaH/NikkaH.hpp"
#include "xhook/xhook.h"
#include "obfuscate.h"
#include "xdl/xdl.h"
#include "il2cpp-class.h"
#include "il2cpp-tabledefs.h"
#include "KittyMemory/MemoryPatch.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Unity/Quaternion.h"
#include "Unity/Unity.h"
#include "Unity/Vector2.h"
#include "Unity/Vector3.h"
#include "Unity/Vector3Silent.h"
#include "Unity/Vector3idk.hpp"
#include "Unity/Vec3.h"

#define S(strn) OBFUSCATE(strn)
#define _(strn) OBFUSCATE(strn)

#if defined (__aarch64__)

#define RETURN _("C0 03 5F D6")
#define TRUE _("20 00 80 D2 C0 03 5F D6")
#define FALSE _("00 00 80 D2 C0 03 5F D6")

#else

#define RETURN _("1E FF 2F E1")
#define TRUE _("01 00 A0 E3 1E FF 2F E1")
#define FALSE _("00 00 A0 E3 1E FF 2F E1")

#endif

uintptr_t il2cpp_base = 0x0;
uintptr_t unity_base = 0x0;
uintptr_t shared_base = 0x0;
uintptr_t egl_base = 0x0;

size_t il2cpp_length = 0x0;
size_t unity_length = 0x0;
size_t shared_length = 0x0;
size_t egl_length = 0x0;

#include "stuff.h"

#include <android/log.h>

enum LogType {
    oDEBUG = 3,
    oERROR = 6,
    oINFO  = 4,
    oWARN  = 5
};

#define TAG "LHIDE"

#ifdef LDEBUG
    #define LOGD(...) ((void)__android_log_print(oDEBUG, TAG, __VA_ARGS__))
    #define LOGE(...) ((void)__android_log_print(oERROR, TAG, __VA_ARGS__))
    #define LOGI(...) ((void)__android_log_print(oINFO,  TAG, __VA_ARGS__))
    #define LOGW(...) ((void)__android_log_print(oWARN,  TAG, __VA_ARGS__))
#else
    #define LOGD(...)
    #define LOGE(...)
    #define LOGI(...)
    #define LOGW(...)
#endif

JavaVM* jvm;

JNIEnv* getEnv() {
    JNIEnv *env;
    int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (status < 0) {
        status = jvm->AttachCurrentThread(&env, NULL);
        if (status < 0) return nullptr;
    }
    return env;
}

bool valid(void* address) {
    if (!address) return false;
    if (((uintptr_t)address) > 0x7FFFFFFFFFFF) return false;
    return true;
}

using namespace nlohmann;

std::string store_getrr;
std::string store_bigl;
std::string olduid = OBFUSCATE("None");

FLog *logger;

bool defchams = false, chams = false, wire = false, glow = false, outline = false, skycolor = false, rainb = false, night = false;
float linewidth = 2.5f;
bool norecoil = false, wallshot = false, bunny = false, ammoh = false, firerate = false, fastk = false, fastbomb = false;
bool isESP = false, ESPSkeleton = false, ESPBox = false, ESPBox2D = false, ESPLine = false, ESPNickname = false, ESPHealth = false;
bool aimbot = false, aimcheck = false;
int cradius = 20;


float rr = 0.0f, gg = 255.0f, bb = 0.0f;
int rgb = 1;
bool rF = true, gF, bF = true, rF_, gF_, bF_;
float redd = 255.0f, greenn = 0.0f, bluee = 255.0f;

void perfRGBChange() {
    switch (rgb) {
        case 0: {
            if (gF) {
                greenn--; 
                if (!greenn) { gF = !gF;  rF_ = !rF_; }
            break; }
        if (redd < 255 && rF_) redd++;
        else { rgb++; redd = 255; rF = !rF; rF_ = !rF_; }
        break; } 
        case 1: {
            if (bF) {
                bluee--; 
                if (!bluee) { bF = !bF; gF_ = !gF_; }
            break; }
        if (greenn < 255 && gF_) greenn++;
        else { rgb++; greenn = 255; gF = !gF; gF_ = !gF_; }
        break; } case 2: {
        if (rF) {
            redd--; 
            if (!redd) { rF = !rF; bF_ = !bF_; }
        break; }
        if (bluee < 255 && bF_) bluee++;
        else { rgb = 0; bluee = 255; bF = !bF; bF_ = !bF_; }
        }
    }
	rr = redd;
	gg = greenn;
	bb = bluee;
}

bool file_exists(const std::string& name) {
  struct stat buffer;   
  return (stat(name.c_str(), &buffer) == 0); 
}

long get_file_size(std::string filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[128];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), OBFUSCATE("%Y-%m-%d | %X"), &tstruct);
    return buf;
}

static size_t writebytes(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    std::string *str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(data), realsize);
    return realsize;
}

std::string get_url(const char* site) {
    CURL *curl = curl_easy_init();
    std::string datastr;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, site);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, std::string(OBFUSCATE("https")).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writebytes);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &datastr);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        CURLcode res = curl_easy_perform(curl);
		char *url = NULL;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
        if (!equals(url, site)) return std::string(OBFUSCATE("0"));
        curl_easy_cleanup(curl);
    }
    return datastr;
}

std::string send_header(const char* site, const char* content) {
    CURL *curl = curl_easy_init();
    std::string datastr;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, site);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, std::string(OBFUSCATE("https")).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writebytes);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &datastr);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, OBFUSCATE("Content-Type: text/plain"));
        chunk = curl_slist_append(chunk, OBFUSCATE("ngrok-skip-browser-warning: true"));
        std::string ctc(string(OBFUSCATE("Value: ")) + string(content));
        chunk = curl_slist_append(chunk, ctc.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        CURLcode res = curl_easy_perform(curl);
		char *url = NULL;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
        if (!equals(url, site)) return std::string(OBFUSCATE("0"));
        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }
    return datastr;
}

std::string CalcMD5(std::string s) {
    std::string result;

    unsigned char hash[MD5_DIGEST_LENGTH];
    char tmp[4];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, s.c_str(), s.length());
    MD5_Final(hash, &md5);
    for (unsigned char i : hash) {
        sprintf(tmp, OBFUSCATE("%02x"), i);
        result += tmp;
    }
    return result;
}

std::string CalcSHA256(std::string s) {
    std::string result;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char tmp[4];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, s.c_str(), s.length());
    SHA256_Final(hash, &sha256);
    for (unsigned char i : hash) {
        sprintf(tmp, OBFUSCATE("%02x"), i);
        result += tmp;
    }
    return result;
}

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    auto *mem = (struct MemoryStruct *) userp;
    mem->memory = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == nullptr) {
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

std::string random_string(int length) {
    const std::string alphabet = OBFUSCATE("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    std::string result;
    std::srand(std::time(0));

    for (int i = 0; i < length; ++i) {
        int index = std::rand() % alphabet.length();
        result += alphabet[index];
    }

    return result;
}

bool loadcfgG = false;
bool savecfgG = false;
std::string g_Token, g_Auth, encryption_key;
bool bValid = false;
static bool show;
bool login_remember = false;
bool auto_click = false;

static std::string _auth = "";
static std::string _token = "";
static bool _userHasLogin = false;
static std::string _keys = "";
bool _risk_fiture;
std::string userKey;
int _version;
int _clientVersion = 9;
int modVersion = 9;
std::string _keys_id;
std::string _devices;


std::string Login(const char *user_key) {
    using json = nlohmann::ordered_json;
    std::string userkey_in_string(user_key);
    char build_id[64] = {0};
    __system_property_get(OBFUSCATE("ro.build.display.id"), build_id);
    char build_hardware[64] = {0};
    __system_property_get(OBFUSCATE("ro.hardware"), build_hardware);

    std::string bKeyID;
    bKeyID.reserve(128);
    bKeyID += build_id;
    bKeyID += build_hardware;

    if (!bKeyID.empty()) {
        size_t pos = bKeyID.find(' ');
        while (pos != std::string::npos) {
            bKeyID.replace(pos, 1, "");
            pos = bKeyID.find(' ', pos);
        }
    }

    std::string UUID = bKeyID;

    std::string errMsg;
    struct MemoryStruct chunk {};
    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    CURL *curl = curl_easy_init();
    if (!curl) {
        return "CURL initialization failed";
    }

    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    std::string sRedLink = OBFUSCATE("https://modkey.space/223/connect");
    curl_easy_setopt(curl, CURLOPT_URL, sRedLink.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, OBFUSCATE("Content-Type: application/x-www-form-urlencoded"));
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    char data[4096];
    sprintf(data, OBFUSCATE("game=Standoff2&user_key=%s&serial=%s"), user_key, UUID.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        try {
            auto result = json::parse(chunk.memory);

            // Проверяем наличие ключа "status" и его значение
            if (result.contains("status") && result["status"].is_boolean() && result["status"]) {
                if (result.contains("data") && result["data"].is_object()) {
                    auto dataObj = result["data"];

                    if (dataObj.contains("token") && dataObj["token"].is_string()) {
                        std::string token = dataObj["token"];
                        g_Token = token;
                    }

                    if (dataObj.contains("rng") && dataObj["rng"].is_number_integer()) {
                        time_t rng = dataObj["rng"];
                        if (rng + 30 > time(0)) {
                            std::string auth = "Standoff2-" + userkey_in_string + "-" + UUID;
                            auth += "-Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E";
                            g_Auth = CalcMD5(auth);
                            bValid = (g_Token == g_Auth);
                        }
                    }
                }
            } else {
                errMsg = result.contains("reason") && result["reason"].is_string()
                             ? result["reason"]
                             : "Unknown error occurred";
            }
        } catch (json::exception &e) {
            errMsg = "{";
            errMsg += e.what();
            errMsg += "}\n{";
            errMsg += chunk.memory;
            errMsg += "}";
        }
    } else {
        errMsg = curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
    return bValid ? "OK" : errMsg;
}

void* myphoton = nullptr;
void *photon = nullptr;
bool (*IsLocal)(void* photon);
int (*GetPlayerHealth)(void* player);
int (*GetHealthPhoton)(void* player);
void (*SetPlayerHealth)(void* photon, int value);
int (*get_width)();
int (*get_height)();
void* myPlayer = nullptr;
float (*UDeltaTime)();
int (*GetPlayerTeam)(void* player);
void *(*get_bipedmap)(void*);
//jobject (*getCtx)(void* thus);
void (*Transform_get_position)(void *transform, Vector3 *out);
void (*Transform_get_position2)(void *transform, Vec3 *out);
void *(*GetPhoton)(void *plr);
int (*getWeaponId)(void* ivar);
void* (*getCurrentWeapon)(void* ivar);
void* (*getWeaponryController)(void* ivar);
void *(*Get_transform)(void *component);
Vector3S (*get_pos) (void *component);
void (*set_pos) (void *component, Vector3S loc);
Vector3S (*get_for)(void *players);
Vector3S (*get_right)(void *players);
monoString* (*get_Nickname)(void* photon);
int (*get_ID)(void* photon);
//void (*SetUID)(void* skaa, monoString* pzdc);
void (*Set_position)(void *transform, Vec3 newPosition);
void (*SetTPS)(void* instance);
void (*SetFPS)(void* instance);
void (*W2S)(void* cam, Vec3 pos, int eye, Vec3 *out);
void* (*GetCamera)();
bool (*get_isObjectAlive)(void*);
void* (*get_gameObject)(void*);
monoString* (*PProps)(void*);

Vector3 get_position(void *player) {
    Vector3 pos;
    Transform_get_position(player, &pos);
    return pos;
}

Vec3 get_positions(void *player) {
    Vec3 pos;
    Transform_get_position2(player, &pos);
    return pos;
}

Vec3 WorldToScreenPoint(void* cam, Vec3 pos) {
    Vec3 ok;
	if (!cam) return Vec3(0,0,0);
    W2S(cam,pos,2,&ok);
    return ok;
}

Vector3 GetPlayerLocation(void *player){
    Vector3 location;
    location = get_position(Get_transform(player));
    return location;
}

Vector3S GetPlayerLocationZ(void *player){
    Vector3 location;
    location = get_position(Get_transform(player));
    return Vector3S(location.X, location.Y, location.Z);
}

Vec3 GetPlayerLoc(void *player){
    Vec3 location;
    location = get_positions(Get_transform(player));
    return location;
}

static void *get_aim(void *player) {
    return *(void **)((uint64_t) player + 0x60);
}

static bool IsMine(void *photon) {
	return *(void **)((uint64_t) photon + 0x30);
}

static void *get_photon(void *player) {
	return *(void **)((uint64_t) player + 0x130);
}

static void *get_hands(void *player) {
    return *(void **)((uint64_t) player + 0x80);
}

/////////

Vec3 GetPlayerHead(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x20);
    return get_positions(bone);
}
Vec3 GetPlayerNeck(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x28);
    return get_positions(bone);
}
Vec3 GetPlayerHip(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x88);
    return get_positions(bone);
}
Vec3 GetPlayerleftLowerArm(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x58);
    return get_positions(bone);
}
Vec3 GetPlayerLeftHand(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x60);
    return get_positions(bone);
}
Vec3 GetPlayerRightHand(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x80);
    return get_positions(bone);
}
Vec3 GetPlayerleftUpperArm(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x50);
    return get_positions(bone);
}
Vec3 GetPlayerrightLowerArm(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x78);
    return get_positions(bone);
}
Vec3 GetPlayerrightUpperArm(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x70);
    return get_positions(bone);
}
Vec3 GetPlayerleftLowerLeg(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0xA0);
    return get_positions(bone);
}
Vec3 GetPlayerleftUpperLeg(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0x90);
    return get_positions(bone);
}
Vec3 GetPlayerrightLowerLeg(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0xC0);
    return get_positions(bone);
}
Vec3 GetPlayerrightUpperLeg(void *player) {
	void* bone = *(void **) ((uint64_t) get_bipedmap(player) + 0xB0);
    return get_positions(bone);
}

///////

bool isPlayerDead(void *player) {
    if (!valid(player)) return true;
    if (GetPlayerHealth(player) < 1) {
        return true;
    }
    return false;
}

struct enemy_t {
    void *object;
    Vector3 location;
    int health;
};
class ESPManager {
public:
    std::vector<enemy_t *> *enemies;
    ESPManager() {
        enemies = new std::vector<enemy_t *>();
    }
    bool isEnemyPresent(void *enemyObject) {
        for (std::vector<enemy_t *>::iterator it = enemies->begin(); it != enemies->end(); it++) {
            if ((*it)->object == enemyObject) {
                return true;
            }
        }
        return false;
    }
    void removeEnemy(enemy_t *enemy) {
        for (int i = 0; i < enemies->size(); i++) {
            if ((*enemies)[i] == enemy) {
                enemies->erase(enemies->begin() + i);
                return;
            }
        }
    }
	void eraseAll() {
		enemies = new std::vector<enemy_t *>();
	}
    void tryAddEnemy(void *enemyObject) {
		if (!valid(enemyObject)) return;
        if (isEnemyPresent(enemyObject)) {
            return;
        }
        if (isPlayerDead(enemyObject)) {
            return;
        }
        enemy_t *newEnemy = new enemy_t();
        newEnemy->object = enemyObject;
        enemies->push_back(newEnemy);
    }
    void updateEnemies(void *enemyObject) {
        for (int i = 0; i < enemies->size(); i++) {
            enemy_t *current = (*enemies)[i];
			if (!valid(current)) return;
			if (!valid(current->object)) return;
            if (isPlayerDead(current->object)) {
                enemies->erase(enemies->begin() + i);
            }
			if (!valid(myPlayer)) return;
            int locteam = GetPlayerTeam(myPlayer);
			if (!valid(current->object)) return;
            int othteam = GetPlayerTeam(current->object);
            if (locteam == othteam) {
                enemies->erase(enemies->begin() + i);
            }
        }
    }
    void removeEnemyGivenObject(void *enemyObject) {
        for (int i = 0; i < enemies->size(); i++) {
            if ((*enemies)[i]->object == enemyObject) {
                enemies->erase(enemies->begin() + i);
                return;
            }
        }
    }

};

ESPManager *players = new ESPManager();
ESPManager *players2 = new ESPManager();

bool setup = false;

float NormalizeAngle (float angle){
    while (angle>360)
        angle -= 360;
    while (angle<0)
        angle += 360;
    return angle;
}
Vector3 NormalizeAngles (Vector3 angles){
    angles.X = NormalizeAngle (angles.X);
    angles.Y = NormalizeAngle (angles.Y);
    angles.Z = NormalizeAngle (angles.Z);
    return angles;
}
Vector3 ToEulerRad(Quaternion q1){
    float Rad2Degz = 360.0f / (M_PI * 2.0f);
    float sqw = q1.W * q1.W;
    float sqx = q1.X * q1.X;
    float sqy = q1.Y * q1.Y;
    float sqz = q1.Z * q1.Z;
    float unit = sqx + sqy + sqz + sqw;
    float test = q1.X * q1.W - q1.Y * q1.Z;
    Vector3 v;
    if (test>0.4995f*unit) {
        v.Y = 2.0f * atan2f (q1.Y, q1.X);
        v.X = M_PI / 2.0f;
        v.Z = 0;
        return NormalizeAngles(v * Rad2Degz);
    }
    if (test<-0.4995f*unit) {
        v.Y = -2.0f * atan2f (q1.Y, q1.X);
        v.X = -M_PI / 2.0f;
        v.Z = 0;
        return NormalizeAngles (v * Rad2Degz);
    }
    Quaternion q(q1.W, q1.Z, q1.X, q1.Y);
    v.Y = atan2f (2.0f * q.X * q.W + 2.0f * q.Y * q.Z, 1 - 2.0f * (q.Z * q.Z + q.W * q.W)); // yaw
    v.X = asinf (2.0f * (q.X * q.Z - q.W * q.Y)); // pitch
    v.Z = atan2f (2.0f * q.X * q.Y + 2.0f * q.Z * q.W, 1 - 2.0f * (q.Y * q.Y + q.Z * q.Z)); // roll
    return NormalizeAngles (v * Rad2Degz);
}

void *GetClosestEnemy(void *local) {
    if (!valid(local)) {
        return NULL;
    }
    float shortestDistance = 99999999.0f;
    float maxAngle = 99999999.0f;
    void* closestEnemy = NULL;
    void* LocalPlayer = local;
    if (valid(LocalPlayer)) {
        for (int i = 0; i < players2->enemies->size(); i++) {
            void *Player = (*players2->enemies)[i]->object;
            if (valid(Player)) {
                if (!isPlayerDead(Player)) {
                Vector3 PlayerPos = GetPlayerLocation(Player);
                Vector3 LocalPlayerPos = GetPlayerLocation(LocalPlayer);
                Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                Vector3S nm = get_for(Get_transform(GetCamera()));
                Vector3 getfor = Vector3(nm.x, nm.y, nm.z);
                float angle = Vector3::Angle(targetDir, getfor) * 100.0;
                if (angle <= maxAngle) {
                    if (angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                }
                }
            }
        }
    }
    return closestEnemy;
}

bool containsValue(const rapidjson::Value& array, int value) {
    for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
        if (array[i].IsInt() && array[i].GetInt() == value) {
            return true;
        }
    }
    return false;
}

template <typename TKey, typename TValue>
struct Dictionary : public monoDictionary<TKey, TValue> {
    typename monoDictionary<TKey, TValue>::Entry* begin() const {
        return this->entries->m_Items;
    }
    typename monoDictionary<TKey, TValue>::Entry* end() const {
        return this->entries->m_Items + this->count;
    }
};

void* GetBoltIService() {
    Il2CppClass* bisclass = GetClassFromA(_("Bolt"), _(""), _("HCHECECGCCFCGAB"));
    LOGI("bisclass %p", bisclass);
    if (!valid(bisclass)) return nullptr;
    FieldInfo* bisinstance = il2cpp_class_get_field_from_name(bisclass, OBFUSCATE("<FDAHGBFFFGCAEHC>k__BackingField"));
    if (!valid(bisinstance)) return nullptr;
	LOGI("bisinstance %p", bisinstance);
    void* ret = nullptr;
    il2cpp_field_static_get_value(bisinstance, &ret);
    if (!valid(ret)) return nullptr;
    return ret;
}

int itemUniqueId = 0;
static bool skinsInitialized = false;

void* (*BoltInventoryItemCtor)(void*);
std::vector<std::pair<int, std::string>> skins;

void ClearInventory(void* boltInventoryService) {
    if (!boltInventoryService) {
        return;
    }
    monoDictionary<int, void*>* InventoryItems = *(monoDictionary<int, void*>**) ((uint64_t) boltInventoryService + 0xF8);
    Il2CppObject* InventoryItems2 = *(Il2CppObject**) ((uint64_t) boltInventoryService + 0xF8);
    if (!InventoryItems) {
        return;
    }
    Il2CppClass* clas2 = InventoryItems2->klass;
    const MethodInfo* clearMethod;
    void* iter = nullptr;
    while (auto method = il2cpp_class_get_methods(clas2, &iter)) {
        auto type = il2cpp_class_from_type(method->return_type);
        if (equals(type->name, _("Void")) && equals(method->name, _("Clear")) && method->parameters_count == 0) {
            clearMethod = method;
        }
    }
    (((void (*)(void *))(clearMethod->methodPointer)))(InventoryItems);
}

void AddAllItemsToInventory(void* boltInventoryService) {
    if (!valid(boltInventoryService)) {
        return;
    }
    ClearInventory(boltInventoryService);
    monoDictionary<int, void*>* InventoryItems = *(monoDictionary<int, void*>**) ((uint64_t)boltInventoryService + 0xF8);
    Il2CppObject* InventoryItems2 = *(Il2CppObject**) ((uint64_t)boltInventoryService + 0xF8);
    if (!valid(InventoryItems)) {
        //logger->append("error inventory items");
        return;
    }
    for (const auto& skin : skins) {
        //LOGI("SKIN: %s, %d,", skin.second.c_str(), skin.first);
        if (contains(skin.second, _("StatTrack")) || contains(skin.second, _("Veteran")) || contains(skin.second, _("Fragment")) || contains(skin.second, _("Scorpion")) || contains(skin.second, _("Kukri")) || contains(skin.second, _("Case")) || contains(skin.second, _("Box")) || contains(skin.second, _("Sticker")) || contains(skin.second, _("Shield")) || contains(skin.second, _("Charm")) || contains(skin.second, _("Chibi")) || contains(skin.second, _("Medal")) || contains(skin.second, _("Graffiti"))) {
            continue;
        }
        if (true) {
            auto itemClass = GetClassFromA(OBFUSCATE("Bolt"), OBFUSCATE(""), OBFUSCATE("CEFHBFACHHBDHAB"));
            Il2CppObject* inventoryItem = il2cpp_object_new(itemClass);
            if (inventoryItem) {
                BoltInventoryItemCtor(inventoryItem);
                itemUniqueId++;
                int uniqueId = itemUniqueId;
                *(int*)((uintptr_t)inventoryItem + 0x30) = uniqueId;
                *(int*)((uintptr_t)inventoryItem + 0x10) = skin.first;
                *(int*)((uintptr_t)inventoryItem + 0x18) = 7;
                if (!InventoryItems->ContainsKey(std::to_string(uniqueId).c_str())) {
                    Il2CppClass* clas2 = InventoryItems2->klass;
                    const MethodInfo* addMethod;
                    void* iter = nullptr;
                    while (auto method = il2cpp_class_get_methods(clas2, &iter)) {
                        auto type = il2cpp_class_from_type(method->return_type);
                        if (equals(type->name, _("Void")) && equals(method->name, _("set_Item")) && method->parameters_count == 2) {
                            addMethod = method;
                        }
                    }
                    (((void (*)(void *, int, void*, const MethodInfo *))(addMethod->methodPointer)))(InventoryItems, uniqueId, inventoryItem, addMethod);
                }
            }
        }
    }
}

void AddItemsFromArray(void* boltInventoryService, std::string jsonStr) {
    if (!valid(boltInventoryService)) {
        return;
    }
    rapidjson::Document skinArray;
    skinArray.Parse(jsonStr.c_str());
    if (skinArray.HasParseError()) {
        LOGI("JSON parsing error!");
        return;
    } else {
        LOGI("JSON parsed!");
    }
    if (!skinArray.HasMember("skins") && !skinArray["skins"].IsArray()) {
        LOGI("Key 'skins' not found or not an array!");
        return;
    }
    ClearInventory(boltInventoryService);
    monoDictionary<int, void*>* InventoryItems = *(monoDictionary<int, void*>**) ((uint64_t)boltInventoryService + 0xF8);
    Il2CppObject* InventoryItems2 = *(Il2CppObject**) ((uint64_t)boltInventoryService + 0xF8);
    if (!valid(InventoryItems)) {
        //logger->append("error inventory items");
        return;
    }
    for (const auto& skin : skins) {
        const rapidjson::Value& skinsArray = skinArray["skins"];
        if (!containsValue(skinsArray, skin.first)) continue;
        if (contains(skin.second, _("StatTrack")) || contains(skin.second, _("Veteran")) || contains(skin.second, _("Fragment")) || contains(skin.second, _("Scorpion")) || contains(skin.second, _("Kukri")) || contains(skin.second, _("Case")) || contains(skin.second, _("Box")) || contains(skin.second, _("Sticker")) || contains(skin.second, _("Shield")) || contains(skin.second, _("Charm")) || contains(skin.second, _("Chibi")) || contains(skin.second, _("Medal")) || contains(skin.second, _("Graffiti"))) {
            continue;
        }
        if (true) {
            auto itemClass = GetClassFromA(OBFUSCATE("Bolt"), OBFUSCATE(""), OBFUSCATE("CEFHBFACHHBDHAB"));
            Il2CppObject* inventoryItem = il2cpp_object_new(itemClass);
            if (inventoryItem) {
                BoltInventoryItemCtor(inventoryItem);
                itemUniqueId++;
                int uniqueId = itemUniqueId;
                *(int*)((uintptr_t)inventoryItem + 0x30) = uniqueId;
                *(int*)((uintptr_t)inventoryItem + 0x10) = skin.first;
                *(int*)((uintptr_t)inventoryItem + 0x18) = 7;
                if (!InventoryItems->ContainsKey(std::to_string(uniqueId).c_str())) {
                    Il2CppClass* clas2 = InventoryItems2->klass;
                    const MethodInfo* addMethod;
                    void* iter = nullptr;
                    while (auto method = il2cpp_class_get_methods(clas2, &iter)) {
                        auto type = il2cpp_class_from_type(method->return_type);
                        if (equals(type->name, _("Void")) && equals(method->name, _("set_Item")) && method->parameters_count == 2) {
                            addMethod = method;
                        }
                    }
                    (((void (*)(void *, int, void*, const MethodInfo *))(addMethod->methodPointer)))(InventoryItems, uniqueId, inventoryItem, addMethod);
                }
            }
        }
    }
}

void*(*set_sfloat)(float);
void*(*set_sint)(int);
void*(*set_sbool)(bool);
int(*get_sint)(void*);

std::string ESPResponse("");

char stats[2048];

char myptr[32];

Il2CppClass* pmgclass;
FieldInfo* pmginstance;

void PUpdate() {
    while (true) {
        if (isESP) {
            if (!valid(pmgclass)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            if (!valid(pmginstance)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            void* playerManager;
            il2cpp_field_static_get_value(pmginstance, &playerManager);
            if (!valid(playerManager)) {
                myPlayer = nullptr;
                players->eraseAll();
                players2->eraseAll();
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                continue;
            }

            auto playerlist = *reinterpret_cast<monoDictionary<int, void*>**>(
                reinterpret_cast<uint64_t>(playerManager) + 0x28);
            if (!valid(playerlist)) {
                myPlayer = nullptr;
                players->eraseAll();
                players2->eraseAll();
                std::this_thread::sleep_for(std::chrono::milliseconds(600));
                continue;
            }

            if (playerlist->getSize() <= 0) {
                myPlayer = nullptr;
                players->eraseAll();
                players2->eraseAll();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }

            for (int i = 0; i < playerlist->getSize(); ++i) {
                auto entry = playerlist->entries->getPointer()[i];
                void* player = entry.value;
                void* localplayer = *(void**)((uint64_t)playerManager + 0x70);
                if (valid(player)) {
                    if (valid(localplayer)) {
                        //sprintf(myptr, OBFUSCATE(" | LocalPlayer: %p"), localplayer);
                        myPlayer = localplayer;
                        void* mcontr = *(void**)((uintptr_t)localplayer + 0x78);
                        if (valid(mcontr)) {
                            *(bool*)((uintptr_t)mcontr + 0x68) = true;
                            void* plrtprms = *(void**)((uintptr_t)mcontr + 0xA8);
                            if (valid(plrtprms)) {
                                void* jmprms = *(void**)((uintptr_t)plrtprms + 0x50);
                                if (valid(jmprms)) {
                                    if (bunny)
                                        *(float*)((uintptr_t)jmprms + 0x60) = 2.1f;
                                }
                            }
                        }
                        if (player != localplayer) {
                            if (valid(localplayer) && valid(player)) {
                                int locteam = 0;
                                int othteam = 0;
                                if (valid(player) || valid(localplayer))
                                    locteam = GetPlayerTeam(localplayer);
                                if (valid(player) || valid(localplayer))
                                    othteam = GetPlayerTeam(player);
                                if (locteam != othteam) {
                                    if (valid(player) || valid(localplayer)) {
                                        players->tryAddEnemy(player);
                                        players2->tryAddEnemy(player);
                                    }
                                }
                                if (valid(player) || valid(localplayer)) {
                                    players->updateEnemies(player);
                                    players2->updateEnemies(player);
                                }
                            }
                        }
                    } else {
                        myPlayer = nullptr;
                    }
                }
            }
        }
        usleep(3900);
    }
}

void GunProcessor() {
    while (true) {
        if (isESP && valid(myPlayer) && (aimbot || ammoh || firerate || norecoil || wallshot || fastk)) {
            if (!isPlayerDead(myPlayer)) {
                void* localplayer = myPlayer;
                void* weaponry = *(void**)((uintptr_t)localplayer + 0x68);
                if (valid(weaponry)) {
                    void* weapon = *(void**)((uintptr_t)weaponry + 0x98);
                    if (valid(weapon)) {
                        if (aimbot || ammoh || firerate || norecoil || wallshot || fastk) {
                            void* wprm = *(void**)((uintptr_t)weapon + 0xA0);
                            if (wprm) {
                                int wpid = *(int*)((uintptr_t)wprm + 0x18);
                                if (wpid) {
                                    if (wpid < 70) {
                                        void* aamo = *(void**)((uintptr_t)weapon + 0x110);
                                        if (aamo) {
                                            aimcheck = true;
                                            if (ammoh)
                                                *(void**)((uintptr_t)weapon + 0x110) = set_sint(5);
                                            if (firerate)
                                                *(void**)((uintptr_t)weapon + 0xF0) = set_sfloat(0);
                                            if (norecoil) {
                                                void* RecoilControl = *(void **)((uint64_t)weapon + 0x148);
                                                if (RecoilControl) {
                                                    *(float *)((uint64_t)RecoilControl + 0x10) = 0.0f,
                                                    *(float *)((uint64_t)RecoilControl + 0x14) = 0.0f;
                                                    *(float *)((uint64_t)RecoilControl + 0x18) = 0.0f;
                                                    *(float *)((uint64_t)RecoilControl + 0x2C) = 0.0f;
                                                    *(float *)((uint64_t)RecoilControl + 0x38) = 0.0f;
                                                    *(float *)((uint64_t)RecoilControl + 0x70) = 0.0f;
                                                    *(Vector2 *)((uint64_t)RecoilControl + 0x1C) = Vector2(0,0);
                                                    *(Vector2 *)((uint64_t)RecoilControl + 0x24) = Vector2(0,0);
                                                    *(Vector2 *)((uint64_t)RecoilControl + 0x30) = Vector2(0,0);
                                                }
                                            }
                                            void* gunp = *(void**)((uintptr_t)weapon + 0x150);
                                            if (gunp) {
                                                if (wallshot) {
                                                    *(void**)((uintptr_t)gunp + 0x258) = set_sint(120000000);
                                                }
                                            }
                                        }
                                    } else {
                                        if (wpid == 100) {
                                            if (fastbomb) {
                                                *(void**)((uintptr_t)wprm + 0x14C) = set_sfloat(1.0f);
                                                *(void**)((uintptr_t)wprm + 0x158) = set_sfloat(1.0f);
                                            }
                                        } else {
                                            if (fastk) {
                                                *(void**)((uintptr_t)wprm + 0x14C) = set_sfloat(0);
                                                *(void**)((uintptr_t)wprm + 0x158) = set_sfloat(0);
                                            }
                                        }
                                        aimcheck = false;
                                    }
                                }
                            }
                        }
                    } else {
                        aimcheck = false;
                    }
                }
            }
        }
        usleep(4170);
    }
}

void AimProcessor() {
    while (true) {
        if (isESP && aimbot && aimcheck) {
            if (!valid(pmgclass)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            if (!valid(pmginstance)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            void* playerManager;
            il2cpp_field_static_get_value(pmginstance, &playerManager);
            if (!valid(playerManager)) {
                myPlayer = nullptr;
                players2->eraseAll();
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                continue;
            }
            auto playerlist = *reinterpret_cast<monoDictionary<int, void*>**>(
                reinterpret_cast<uint64_t>(playerManager) + 0x28);
            if (!valid(playerlist)) {
                myPlayer = nullptr;
                players2->eraseAll();
                std::this_thread::sleep_for(std::chrono::milliseconds(600));
                continue;
            }
            if (players2->enemies->size() <= 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }
            if (valid(myPlayer)) {
                if (!isPlayerDead(myPlayer)) {
                    void* aimingdata = *(void**)((uint64_t)get_aim(myPlayer) + 0x90);
                    if (aimingdata && valid(aimingdata)) {
                        void* ePlayer = GetClosestEnemy(myPlayer);
                        if (valid(ePlayer)) {
                            if (!isPlayerDead(ePlayer)) {
                                Vector3S camloc = Vector3S(GetPlayerLocation(GetCamera()).X, GetPlayerLocation(GetCamera()).Y, GetPlayerLocation(GetCamera()).Z);
                                Vec3 headpos = GetPlayerHead(ePlayer);
                                Vector3S target = (Vector3S(headpos.x, headpos.y, headpos.z) + Vector3S(0, 0, 0)) - camloc;
                                float aimAngle = Vector3S::Angle(target, get_for(Get_transform(GetCamera())) * 100);
                                void* player_character_view = *(void **)((uint64_t)ePlayer + 0x48);
                                bool visible = *(bool *)((uint64_t)player_character_view + 0x30);
                                if (visible && (aimAngle <= cradius / 360.0f)) {
                                    Vector3 direct = Vector3::Normalized((Vector3(headpos.x, headpos.y, headpos.z) + Vector3(0, -1.4f, 0)) - GetPlayerLocation(myPlayer));
                                    Quaternion PlayerLook = Quaternion::LookRotation(direct, Vector3(0, 1, 0));
                                    Vector3 angle = ToEulerRad(PlayerLook);
                                    //sprintf(stats, " | X: %f, Y: %f, Z: %f", angle.X, angle.Y, angle.Z);
                                    if (angle.X >= 275.0f) angle.X -= 360.0f;
                                    if (angle.X <= -275.0f) angle.X += 360.0f;
                                    *(Vector3*)((uint64_t)aimingdata + 0x18) = angle;
                                    *(Vector3*)((uint64_t)aimingdata + 0x24) = angle;
                                    usleep(4170);
                                    continue;
                                }
                            }
                        }
                    } else {
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    }
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
        usleep(4170);
    }
}

void EspProcessor() {
    struct sockaddr_in serverAddr;
    serverAddr.sin_port = htons(atoi(OBFUSCATE("19133")));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    while (true) {
        if (!isESP) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            continue;
        }
        rapidjson::Document Enemies;
        Enemies.SetObject();
        rapidjson::Document::AllocatorType& allocator = Enemies.GetAllocator();
        Enemies.AddMember("event", "esp", allocator);
        int realc = 0;
        for (int i = 0; i < players->enemies->size(); i++) {
            void *Player = (*players->enemies)[i]->object;
            if (valid(Player)) {
                bool isdead = isPlayerDead(Player);
                if (isdead) continue;
                void* mcamera = GetCamera();
                if (!valid(mcamera)) continue;
                float posnum = 1.0; // Height slider * 0.1
                float posnum2 = 1.0; // Size slider * 0.1
                Rect rect;
                Vec3 PlayerPos = GetPlayerLoc(Player);
                Vec3 PosNew = {0.f, 0.f, 0.f};
                PosNew = WorldToScreenPoint(mcamera, PlayerPos);
                if (PosNew.z < 1.f) continue;
                if (!valid(mcamera)) continue;
                Vec3 Origin = PlayerPos;
                Origin.y += 0.7f;
                Origin.y += posnum;
                Vec3 BoxPosNew = {0.f, 0.f, 0.f};
                BoxPosNew = WorldToScreenPoint(mcamera, Origin);
                float Height = abs(BoxPosNew.y - PosNew.y) * (posnum2 / posnum), 
                Width = Height * 0.60f;
                rect = Rect(BoxPosNew.x - Width / 2.f, (get_height() - BoxPosNew.y), Width, Height);
                rapidjson::Value playerData(rapidjson::kObjectType);
                if (ESPHealth) {
                    int health = GetHealthPhoton(GetPhoton(Player));
                    if (health < 0) health = 0;
                    playerData.AddMember("hp", health, allocator);
                }
                if (ESPNickname) {
                    std::string nname = string_to_hex(get_Nickname(GetPhoton(Player))->toChars());
                    playerData.AddMember("nk", rapidjson::Value(nname.c_str(), allocator), allocator);
                }
                rapidjson::Value rc(rapidjson::kObjectType);
                rc.AddMember("x", rect.x, allocator);
                rc.AddMember("y", rect.y, allocator);
                rc.AddMember("w", rect.width, allocator);
                rc.AddMember("h", rect.height, allocator);
                playerData.AddMember("rc", rc, allocator);
                Enemies.AddMember(rapidjson::Value(std::to_string(realc).c_str(), allocator).Move(), playerData, allocator);
                realc++;
            }
        }
        Enemies.AddMember("size", realc, allocator);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        Enemies.Accept(writer);
        std::string jsonString = buffer.GetString();
        std::string encrypted = string_to_hex(xor_cipher(jsonString, OBFUSCATE("System.Reflection"), true));
        ssize_t isent = sendto(serverSocket, encrypted.c_str(), encrypted.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));   
        usleep(8000);
    }
}


///

Il2CppClass* WeaponWithSkin;

int wid = 0, sid = 0;
bool setwp = false;
bool errata = false;

MethodInfo* gupdate;

void (*SetWeapon)(void* _this, void* obj);
void (*SetWeaponID)(void*, int);

void(*old_gameupdate)(void *);
void gameupdate(void* inst) {
    if (setwp) {
		//LOGD("SET %p, %d", inst, wid);
        SetWeaponID(inst, wid);
        setwp = false;
    }
    old_gameupdate(inst);
	errata = true;
}

///

MemoryPatch chamsbp;

void* (*GetBytes)(void*, monoString*);
monoString* (*GetString)(void*, void*);
void* (*DefaultEncoding)();

void mnthread() {
    LOGI("PART 1");
	logger = new FLog(_("/sdcard/Documents/log.txt"));
    LOGI("PART 2");
	//logger->append(_("begin"));
    LOGI("PART 3");
	sprintf(myptr, _(" | Empty"));
    LOGI("PART 4 %s %p", myptr, myptr);
	//logger->append_arg(_("the beggining of bing bang, Build Date: %s | %s"), __DATE__, __TIME__);
	LOGI("PART 5");
	do {
		usleep(1);
	} while (!isLibraryLoaded(Il2CppPath));
    
    LOGI("PART 6");
	
	init_api();
	
	//LOGI(_("il2cpp passed!"));
	
	do {
		xdl_iterate_phdr(callback_z, NULL, XDL_FULL_PATHNAME);
		usleep(1);
	} while (!shared_base);
    
    xdl_iterate_phdr(callback_z, NULL, XDL_FULL_PATHNAME);
    
    //LOGI(_("start of the universe!.."));
	
	//DobbyHook(xdl_sym(xdl_open(_("libGLESv2.so"), XDL_DEFAULT), _("glDrawElements"), NULL), (void*)new_glDrawElements, (void**)&old_glDrawElements);
    //DobbyHook(xdl_sym(xdl_open(_("libGLESv2.so"), XDL_DEFAULT), _("glGetUniformLocation"), NULL), (void*)new_glGetUniformLocation, (void**)&old_glGetUniformLocation);
    //DobbyHook(xdl_sym(xdl_open(_("libEGL.so"), XDL_DEFAULT), _("eglSwapBuffers"), NULL), (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
    
    //Toast(getEnv(), "Loaded!", 1);
    
    xdl_iterate_phdr(callback_z, NULL, XDL_FULL_PATHNAME);
	
	//LOGI(_("patching! v2"));
    
    /*MemoryPatch b1 = MemoryPatch::createWithHex((il2cpp_base + 0xE8939F), string_to_hex("Nigger.Deflectors")); //System.Reflection
    MemoryPatch b2 = MemoryPatch::createWithHex((il2cpp_base + 0xE6D86F), string_to_hex("Nigger")); //System
    MemoryPatch b3 = MemoryPatch::createWithHex((il2cpp_base + 0xE839B1), string_to_hex("Suka")); //Type
    MemoryPatch b4 = MemoryPatch::createWithHex((il2cpp_base + 0xE81CF6), string_to_hex("YleQalav")); //Assembly
    MemoryPatch b5 = MemoryPatch::createWithHex((il2cpp_base + 0xE71410), string_to_hex("YleDumebi")); //AppDomain
    
    b1.Modify();
    b2.Modify();
    b3.Modify();
    b4.Modify();
    b5.Modify();*/
	
	xdl_iterate_phdr(callback_z, NULL, XDL_FULL_PATHNAME);
	
	il2cpp_field_static_get_value = reinterpret_cast<void(*)(FieldInfo*, void*)>(il2cpp_base + 0x2160580);
	
	pmgclass = GetClassFromA(_("Assembly-CSharp"), _("Axlebolt.Standoff.Player"), _("PlayerManager"));
	LOGI("pmgclass %p", pmgclass);
	pmginstance = il2cpp_class_get_field_from_name(pmgclass, OBFUSCATE("HHFGHDDHHFEBGHH"));
	LOGI("pmginstance %p", pmginstance);
	
    std::string ams[] = {
        "Assembly-Csharp", "mscorlib", "Bolt.Api",
        "System", "Bolt", "System.Data", "UnityEngine.CoreModule",
        "System.Core", "Axlebolt.Warden.Android",
        "Photon3Unity3D"
    };
    LOGI("PASSED ARRAY");
    
    if (!il2cpp_thread_current()) il2cpp_thread_attach(il2cpp_domain_get());
    LOGI("PASSED THROUGHT THREAD CURRENT %p", il2cpp_thread_current);
    
    //logger->append("passed thread");
    
    for (auto& asem : ams) {
        auto assemblie = il2cpp_domain_assembly_open(il2cpp_domain_get(), asem.c_str());
        if (!assemblie) {
            LOGI("Assembly %s.dll not found! Pointer was: %p", asem.c_str(), assemblie);
            continue;
        } else {
			LOGI("Assembly %s.dll found! Pointer : %p", asem.c_str(), assemblie);
		}
        auto image = il2cpp_assembly_get_image(assemblie);
        auto imageName = std::string(image->name);
        auto pos = imageName.rfind('.');
        auto imageNameNoExt = imageName.substr(0, pos);
        auto assemblyFileName = il2cpp_string_new(imageNameNoExt.c_str());
        typedef void *(*Assembly_Load_ftn)(Il2CppString*, void*);
        typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void*, void*);
        auto reflectionAssembly = ((Assembly_Load_ftn)(il2cpp_base + 0x40AEDD0))(assemblyFileName, nullptr);
        auto reflectionTypes = ((Assembly_GetTypes_ftn)(il2cpp_base + 0x40AEBD4))(reflectionAssembly, nullptr);
        monoArray<void*>* items = (monoArray<void*>*)reflectionTypes;
        for (int j = 0; j < items->getCapacity(); ++j) {
            auto klass = il2cpp_class_from_system_type((Il2CppReflectionType*)items->getPointer()[j]);
            if (equals(klass->name, "Encoding")) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Byte[]")) && equals(method->name, _("GetBytes")) && method->parameters_count == 1) {
                        if (equals(il2cpp_class_from_type(method->parameters[0])->name, _("String"))) {
                            GetBytes = (void* (*)(void*, monoString*))method->methodPointer;
                        }
                    } else if (equals(type->name, _("String")) && equals(method->name, _("GetString")) && method->parameters_count == 1) {
                        if (equals(il2cpp_class_from_type(method->parameters[0])->name, _("Byte[]"))) {
                            GetString = (monoString* (*)(void*, void*))method->methodPointer;
                        }
                    } else if (equals(type->name, _("Encoding")) && equals(method->name, _("CreateDefaultEncoding")) && method->parameters_count == 0) {
                        DefaultEncoding = (void* (*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, "Instance")) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("IntPtr")) && equals(method->name, _("_Unwind_GetA")) && method->parameters_count == 0) {
                        //DobbyHook((void*)method->methodPointer, (void*)new_geta, (void**)&old_geta);
                    } else if (equals(type->name, _("IntPtr")) && equals(method->name, _("_Unwind_GetRR"))) {
                        //DobbyHook((void*)method->methodPointer, (void*)_Unwind_GetRR, (void**)&Unwind_GetRR);
                    }
                }
            } else if (equals(klass->name, "ECBFBDHCBGGEADH")) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Task")) && method->slot == 22 && method->parameters_count == 4) {
                        //DobbyHook((void*)method->methodPointer, (void*)new_Task, (void**)&old_Task);
                    }
                }
            } else if (equals(klass->name, _("PlayerController"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && method->parameters_count == 0 && method->slot == 66) {
                        //MemoryPatch::createWithHex((uintptr_t)method->methodPointer, RETURN).Modify();
                        //logger->append("chams bp hooked!");
                    } else if (equals(type->name, _("PhotonPlayer")) && method->parameters_count == 0) {
                        GetPhoton = (void* (*)(void *)) method->methodPointer;
                    } else if (equals(type->name, _("BipedMap")) && method->parameters_count == 0) {
                            get_bipedmap = (void* (*)(void *)) method->methodPointer;
                    } else if (equals(type->name, _("HHHECFACFCHBHAH")) && method->parameters_count == 0) {
                        GetPlayerTeam = (int (*)(void *)) method->methodPointer;
                    } else if (equals(type->name, _("Void")) && method->parameters_count == 0 && method->slot == 64) {
                        //MemoryPatch::createWithHex((uintptr_t)method->methodPointer, RETURN).Modify();
                    }
                }
            } else if (equals(klass->name, _("GameController"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _("Update")) && method->parameters_count == 0) {
						//old_gameupdate = (void(*)(void *))method->methodPointer;
						//gupdate = (MethodInfo*)method->methodPointer;
                        //DobbyHook((void*)method->methodPointer, (void*)gameupdate, (void**)&old_gameupdate);
                    }
                }
            } else if (equals(klass->name, _("Hashtable"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("String")) && equals(method->name, _("ToString")) && method->parameters_count == 0) {
                        PProps = (monoString* (*)(void*))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("Time"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Single")) && equals(method->name, _("get_deltaTime")) && method->parameters_count == 0) {
                        UDeltaTime = (float (*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("Screen"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Int32")) && equals(method->name, _("get_width")) && method->parameters_count == 0) {
                        get_width = (int (*)())method->methodPointer;
                    } else if (equals(type->name, _("Int32")) && equals(method->name, _("get_height")) && method->parameters_count == 0) {
                        get_height = (int (*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("Transform"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Vector3")) && equals(method->name, _("get_position")) && method->parameters_count == 0) {
                    get_pos = (Vector3S (*)(void *))method->methodPointer;
                } else if (equals(type->name, _("Void")) && equals(method->name, _("set_position")) && method->parameters_count == 1) {
                    if (equals(il2cpp_class_from_type(method->parameters[0])->name, _("Vector3"))) {
                        set_pos = (void (*)(void *, Vector3S))method->methodPointer;
                    }
                } else if (equals(type->name, _("Vector3")) && equals(method->name, _("get_forward")) && method->parameters_count == 0) {
                    get_for = (Vector3S (*)(void *))method->methodPointer;
                } else if (equals(type->name, _("Vector3")) && equals(method->name, _("get_right")) && method->parameters_count == 0) {
                    get_right = (Vector3S (*)(void *))method->methodPointer;
                } else if (equals(type->name, _("Void")) && equals(method->name, _("get_position_Injected")) && method->parameters_count == 1) {
                    if (method->parameters[0]->byref) {
                        if (method->parameters[0]->attrs & PARAM_ATTRIBUTE_OUT && !(method->parameters[0]->attrs & PARAM_ATTRIBUTE_IN)) {
                            if (equals(il2cpp_class_from_type(method->parameters[0])->name, _("Vector3"))) {
                                Transform_get_position = (void (*)(void *, Vector3 *))method->methodPointer;
                                Transform_get_position2 = (void (*)(void *, Vec3 *))method->methodPointer;
                            }
                        }
                    }
                } else if (equals(type->name, _("Void")) && equals(method->name, _("set_position_Injected")) && method->parameters_count == 1) {
                    if (method->parameters[0]->byref) {
                        if (equals(il2cpp_class_from_type(method->parameters[0])->name, _("Vector3"))) {
                            Set_position = (void (*)(void *, Vec3))method->methodPointer;
                        }
                    }
                }
                }
            } else if (equals(klass->name, _("Camera"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _("WorldToScreenPoint_Injected")) && method->parameters_count == 3) {
                        W2S = (void (*)(void *, Vec3, int, Vec3 *))method->methodPointer;
                    } else if (equals(type->name, _("Camera")) && equals(method->name, _("get_main")) && method->parameters_count == 0) {
                        GetCamera = (void *(*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("PhotonPlayer"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("String")) && equals(method->name, _("get_NickName")) && method->parameters_count == 0) {
                    get_Nickname = (monoString* (*)(void *))method->methodPointer;
                } else if (equals(type->name, _("Int32")) && equals(method->name, _("get_ID")) && method->parameters_count == 0) {
                    get_ID = (int (*)(void *))method->methodPointer;
                } else if (equals(type->name, _("Boolean")) && equals(method->name, _("get_isLocal")) && method->parameters_count == 0) {
                    IsLocal = (bool (*)(void *))method->methodPointer;
                }
                }
            } else if (equals(klass->name, _("Component"))) {
                void* iter = NULL;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Transform")) && equals(method->name, _("get_transform")) && method->parameters_count == 0) {
                    Get_transform = (void *(*)(void *))method->methodPointer;
                } else if (equals(type->name, _("GameObject")) && equals(method->name, _("get_gameObject")) && method->parameters_count == 0) {
                    get_gameObject = (void *(*)(void*))method->methodPointer;
                }
                }
            } 
        }
    }
    /*
    WeaponWithSkin = GetClassFromName(_(""), _("EBDGACCFEAHAHDD"));
    
    SetWeapon = (void (*)(void*, void*))(il2cpp_base + 0x224AF40);
    SetWeaponID = (void (*)(void*, int))(il2cpp_base + 0x231C1CC);*/
    
    BoltInventoryItemCtor = (void* (*)(void*))(il2cpp_base + 0x2D39984);
    
    set_sfloat = (void* (*)(float))(il2cpp_base + 0x2948EF8);
    set_sint = (void* (*)(int))(il2cpp_base + 0x24EE5CC);
    set_sbool = (void* (*)(bool))(il2cpp_base + 0x2800114);
    get_sint = (int (*)(void*))(il2cpp_base + 0x24ED624);
    
    GetPlayerHealth = (int (*)(void *))(il2cpp_base + 0x28763B4);
    GetHealthPhoton = (int (*)(void *))(il2cpp_base + 0x24729AC);
    
    if (il2cpp_thread_current()) il2cpp_thread_detach(il2cpp_thread_current());
    
    /*b1.Restore();
    b2.Restore();
    b3.Restore();
    b4.Restore();
    b5.Restore();*/
    
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        LOGI(_("Error creating socket"));
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(OBFUSCATE("19132")));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		LOGI(_("Error binding socket"));
		system(std::string(std::string("kill ") + std::to_string(getpid())).c_str());
		return;
	}

    LOGI(_("Server listening on port..."));

	std::thread(PUpdate).detach();
    std::thread(EspProcessor).detach();
    std::thread(AimProcessor).detach();
    std::thread(GunProcessor).detach();
    
    LOGI(_("Successfully launched sub-threads"));
	
	while (true) {
		struct sockaddr_in clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
        char buffer[1024];
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesRead <= 0) {
            continue;
        }
        
        LOGI("GOT SOMETHING");

        buffer[bytesRead] = '\0';
        
        LOGI("NULLIFY NEW CHAR ARRAY SUCCESS %s", buffer);

        LOGI("append success");
        std::string dval(xor_cipher(hex_to_string(std::string(buffer)), OBFUSCATE("System.Reflection"), false));
        LOGI("dec success %p, %s", &dval, dval.c_str());
			if (contains(dval, _("event"))) {
                LOGI("message to parse: %p, %s", &dval, dval.c_str());
				json data = json::parse(dval);
                LOGI("done parsing");
				if (equals(RPB(data["event"].dump()), _("button"))) {
					if (equals(RPB(data["name"].dump()), _("chams"))) {
						glow = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("setwp"))) {
						setwp = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("isesp"))) {
                        isESP = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("espbox"))) {
                        ESPBox2D = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("espline"))) {
                        ESPLine = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("esphp"))) {
                        ESPHealth = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("espskt"))) {
                        ESPSkeleton = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("espnick"))) {
                        ESPNickname = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("aimbot"))) {
                        aimbot = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("wallshot"))) {
                        wallshot = data["state"].get<bool>();
                    } else if (equals(RPB(data["name"].dump()), _("recoil"))) {
                        norecoil = data["state"].get<bool>();
                    } else if (equals(RPB(data["name"].dump()), _("bunny"))) {
                        bunny = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("ammoh"))) {
                        ammoh = data["state"].get<bool>();
                    } else if (equals(RPB(data["name"].dump()), _("firerate"))) {
                        firerate = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("fastk"))) {
                        fastk = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("fastbomb"))) {
                        fastbomb = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("addskin"))) {
                        void* v_bis = GetBoltIService();
                        if (!valid(v_bis)) continue;
                        if (!skinsInitialized) {
                            monoDictionary<int, void*>* inventoryDefinitions = *(monoDictionary<int, void*>**)((uint64_t)v_bis + 0xE8);
                            if (inventoryDefinitions) {
                                std::set<std::string> uniqueSkinNames;
                                for (int i = 0; i < inventoryDefinitions->getSize(); ++i) {
                                    auto entry = inventoryDefinitions->entries->getPointer()[i];
                                    auto key = entry.key;
                                    auto boltInventoryItemDefinition = entry.value;
                                    if (boltInventoryItemDefinition) {
                                        monoString* itemName = *(monoString**)((uintptr_t)boltInventoryItemDefinition + 0x18);
                                        std::string skinName = itemName->toChars();
                                        if (uniqueSkinNames.find(skinName) != uniqueSkinNames.end())
                                            skinName += std::string(_(" StatTrack"));
                                        else
                                            uniqueSkinNames.insert(skinName);    
                                        skins.push_back({key, skinName});
                                    }
                                }
                            }
                            skinsInitialized = true;
                        }
                        if (skins.empty()) skinsInitialized = false;
                        AddAllItemsToInventory(v_bis);
					} else if (equals(RPB(data["name"].dump()), _("jskin"))) {
                        LOGI("bs %p", GetBoltIService);
                        void* v_bis = GetBoltIService();
                        LOGI("v_bis %p", v_bis);
                        if (!valid(v_bis)) continue;
                        /*std::ifstream file(std::string(OBFUSCATE("/sdcard/Documents/doc.json")));
                        if (!file.is_open()) {
                            LOGI("Error opening json file.");
                            continue;
                        }
                        std::stringstream buffer;
                        buffer << file.rdbuf();
                        std::string jsonStr = buffer.str();*/
                        std::string jsonVal(OBFUSCATE("7B22736B696E73223A205B34343030372C2034353030312C2038353130342C203232303031352C2034373530322C203137303032322C203133313530302C2038363331382C2034383030322C2037313730312C2036373730332C2038343930302C2031313030322C20333030332C2037333030342C2037323030332C2037373831352C203133353330302C203133343730302C2034343630332C2036353230322C203132343330302C203133363430302C2037313030342C2037333631322C2039333430302C2031333030332C2038333531322C2038373932312C2036323030332C203232303032332C203133383030312C2036313630312C2033323030352C2031323030315D7D"));
                        std::string jsonStr(hex_to_string(jsonVal));
                        LOGI("json %p", &jsonVal);
                        if (!skinsInitialized) {
                            monoDictionary<int, void*>* inventoryDefinitions = *(monoDictionary<int, void*>**)((uint64_t)v_bis + 0xE8);
                            if (inventoryDefinitions) {
                                std::set<std::string> uniqueSkinNames;
                                for (int i = 0; i < inventoryDefinitions->getSize(); ++i) {
                                    auto entry = inventoryDefinitions->entries->getPointer()[i];
                                    auto key = entry.key;
                                    auto boltInventoryItemDefinition = entry.value;
                                    if (boltInventoryItemDefinition) {
                                        monoString* itemName = *(monoString**)((uintptr_t)boltInventoryItemDefinition + 0x18);
                                        std::string skinName = itemName->toChars();
                                        if (uniqueSkinNames.find(skinName) != uniqueSkinNames.end())
                                            skinName += std::string(_(" StatTrack"));
                                        else
                                            uniqueSkinNames.insert(skinName);
                                            LOGI("pushing %s", skinName.c_str());
                                        skins.push_back({key, skinName});
                                    }
                                }
                            }
                            skinsInitialized = true;
                            LOGI("ski %p", &skinsInitialized);
                        }
                        if (skins.empty()) skinsInitialized = false;
                        AddItemsFromArray(v_bis, jsonStr);
					} else if (equals(RPB(data["name"].dump()), _("clearskin"))) {
                        void* v_bis = GetBoltIService();
                        if (!valid(v_bis)) continue;
                        ClearInventory(v_bis);
                    }
				} else if (equals(RPB(data["event"].dump()), _("value"))) {
					if (equals(RPB(data["name"].dump()), _("wid"))) {
						wid = data["value"].get<int>();
					} else if (equals(RPB(data["name"].dump()), _("sid"))) {
						sid = data["value"].get<int>();
					} else if (equals(RPB(data["name"].dump()), _("cradius"))) {
                        cradius = data["value"].get<int>();
					}
				}
			}
    }
}

__attribute__((__visibility__("default")))
void awaken() {
    LOGI("HELLLOOOOOO");
	std::thread(mnthread).detach();
}
