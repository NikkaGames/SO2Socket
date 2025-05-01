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
#include <cstdint>
#include <cstring>
#include <cstring>
#include <cwchar>
#include <endian.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <csignal>
#include <sys/system_properties.h>
#include <random>
#include <sys/syscall.h>

#include "AES.h"
#include "base64.h"
#include "dobby.h"
#include "FLog.h"
#include "hide.h"
#include "json.hpp"
#include "NikkaH/NikkaH.hpp"
#include "xhook/xhook.h"
#include "obfuscate.h"
#include "xdl/xdl.h"
#include "il2cpp-class-new.h"
#include "il2cpp-tabledefs.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyUtils.h"
#include "KittyMemory/KittyMemory.h"

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
#define NOP _("1F 20 03 D5")
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

//#define LDEBUG

#define TAG "SThread"

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

#define LOGT(...) ((void)__android_log_print(oDEBUG, OBFUSCATE("DSystemServer"), __VA_ARGS__))

void recurseForever(int x) {
    if (x < 0) return;
    recurseForever(x + 1);
}

template<int N>
struct Bloat {
    static inline int compute(int x) {
        return Bloat<N - 1>::compute(x * x + N);
    }
};

template<>
struct Bloat<0> {
    static inline int compute(int x) {
        return x;
    }
};

JavaVM* jvm;

JNIEnv* getEnv() {
    JNIEnv *env;
    int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (status < 0) {
        status = jvm->AttachCurrentThread(&env, nullptr);
        if (status < 0) return nullptr;
    }
    return env;
}

bool valid(void* address) {
    if (!address) return false;
    if (((uintptr_t)address) > 0x7FFFFFFFFFFF) return false;
    return true;
}

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
bool checkc() {
    recurseForever(1);
    volatile int result = Bloat<1000>::compute(42);
    volatile int x = 1;
    for (int i = 0; i < 10000; i++) {
        x = (x * 123456789 + 987654321) % 1000000007;
        x ^= (x << 13) | (x >> 7);
        x += (x * x) ^ 0xDEADBEEF;
    }
    int fd = open(base64_decode(OBFUSCATE("L3N5c3RlbS9ldGMvaG9zdHM=")).c_str(), O_RDONLY);
    if (fd < 0) {
        LOGI("Cannot open file");
        return atoi(OBFUSCATE("1"));
    }
    struct stat st{};
    if (fstat(fd, &st) != 0 || st.st_size <= 0) {
        close(fd);
        LOGI("Cannot open file");
        return atoi(OBFUSCATE("1"));
    }
    std::vector<char> buffer(st.st_size);
    if (read(fd, buffer.data(), st.st_size) != st.st_size) {
        close(fd);
        LOGI("Cannot open file");
        return atoi(OBFUSCATE("1"));
    }
    close(fd);
    if (contains(buffer.data(), base64_decode(OBFUSCATE("Z2l0aHVi"))) || contains(buffer.data(), base64_decode(OBFUSCATE("bW9ka2V5")))) {
        return atoi(OBFUSCATE("1"));
    }
    return atoi(OBFUSCATE("0"));
}

using namespace nlohmann;

std::string store_getrr;
std::string store_bigl;
std::string olduid = OBFUSCATE("None");

FLog *logger;

bool defchams = false, chams = false, wire = false, glow = false, outline = false, skycolor = false, rainb = false, night = false;
float linewidth = 2.5f;
bool norecoil = false, wallshot = false, bunny = false, ammoh = false, firerate = false, fastk = false, fastbomb = false, ugrenade = false, gnuke = false, mvbfr = false;
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

long get_file_size(const std::string& filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

char *read_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return nullptr;
    struct stat st{};
    if (fstat(fd, &st) < 0) {
        close(fd);
        return nullptr;
    }
    char *buffer = (char*)malloc(st.st_size + 1);
    if (!buffer) {
        close(fd);
        return nullptr;
    }
    if (read(fd, buffer, st.st_size) != st.st_size) {
        free(buffer);
        close(fd);
        return nullptr;
    }
    buffer[st.st_size] = '\0';
    close(fd);
    return buffer;
}

std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[128];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), OBFUSCATE("%Y-%m-%d | %X"), &tstruct);
    return buf;
}

std::string random_string(int length) {
    const std::string alphabet = OBFUSCATE("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    std::string result;
    std::srand(std::time(nullptr));

    for (int i = 0; i < length; ++i) {
        int index = std::rand() % alphabet.length();
        result += alphabet[index];
    }

    return result;
}

/*void* get_GameInstance() {
    void* ret;
    Il2CppClass *cls = GetClassFromName(_("Axlebolt.Standoff.Game"), _("GameManager"));
    FieldInfo* fnm = il2cpp_class_get_field_from_name(cls, _("AEHADBBCHGHHHCB"));
    il2cpp_field_static_get_value(fnm, &ret);
    void* rets = *(void**)((uintptr_t)ret + 0x58);
    return rets;
}*/

void* myphoton = nullptr;
void *photon = nullptr;
bool (*IsLocal)(void* photon);
int (*GetPlayerHealth)(void* player);
int (*GetHealthPhoton)(void* player);
void (*SetPlayerHealth)(void* photon, int value);
void* (*GetBytes)(void*, monoString*);
monoString* (*GetString)(void*, void*);
void* (*DefaultEncoding)();
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
MethodInfo* (*GetMethodInfo)(Il2CppType*,monoString*);
void (*SetWeapon)(void* _this, void* obj);
void (*SetWeaponID)(void*, int);

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

Vec3 WorldToScreenPoint(void* cam, const Vec3& pos) {
    Vec3 ok;
	if (!cam) return {0,0,0};
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
    return {location.X, location.Y, location.Z};
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
    void* photon = GetPhoton(player);
    if (photon) {
        try {
            int hp = GetHealthPhoton(photon);
            if (hp <= 0) return true;
        } catch (...) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

struct enemy_t {
    void *object{};
    Vector3 location;
    int health{};
};
class ESPManager {
public:
    std::vector<enemy_t *> *enemies;
    ESPManager() {
        enemies = new std::vector<enemy_t *>();
    }
    bool isEnemyPresent(void *enemyObject) const {
        return std::any_of(enemies->begin(), enemies->end(), [enemyObject](enemy_t* enemie) {
            return enemie->object == enemyObject;
        });
    }
    void removeEnemy(enemy_t *enemy) const {
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
    void tryAddEnemy(void *enemyObject) const {
		if (!valid(enemyObject)) return;
        if (isEnemyPresent(enemyObject)) {
            return;
        }
        if (isPlayerDead(enemyObject)) {
            return;
        }
        auto *newEnemy = new enemy_t();
        newEnemy->object = enemyObject;
        enemies->push_back(newEnemy);
    }
    void updateEnemies(void *enemyObject) const {
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
    void removeEnemyGivenObject(void *enemyObject) const {
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
        return nullptr;
    }
    float shortestDistance = 99999999.0f;
    float maxAngle = 99999999.0f;
    void* closestEnemy = nullptr;
    void* LocalPlayer = local;
    if (valid(LocalPlayer)) {
        for (auto & enemie : *players2->enemies) {
            void *Player = enemie->object;
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

Il2CppClass* Vec3Cls;
Il2CppClass* bisclass;

void* GetBoltIService() {
    LOGI("bisclass %p", bisclass);
    if (!valid(bisclass)) return nullptr;
    FieldInfo* bisinstance = il2cpp_class_get_field_from_name(bisclass, OBFUSCATE("<GFDBBFBAEDAGADA>k__BackingField"));
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

Il2CppClass* itemClass;

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

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
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
        if (contains(skin.second, _("Crate")) || contains(skin.second, _("StatTrack")) || contains(skin.second, _("Veteran")) || contains(skin.second, _("Fragment")) || contains(skin.second, _("Scorpion")) || contains(skin.second, _("Kukri")) || contains(skin.second, _("Case")) || contains(skin.second, _("Box")) || contains(skin.second, _("Sticker")) || contains(skin.second, _("Shield")) || contains(skin.second, _("Charm")) || contains(skin.second, _("Chibi")) || contains(skin.second, _("Medal")) || contains(skin.second, _("Graffiti"))) {
            continue;
        }
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

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
void AddItemsFromArray(void* boltInventoryService, const std::string& jsonStr) {
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
        if (contains(skin.second, _("Crate")) || contains(skin.second, _("StatTrack")) || contains(skin.second, _("Veteran")) || contains(skin.second, _("Fragment")) || contains(skin.second, _("Scorpion")) || contains(skin.second, _("Kukri")) || contains(skin.second, _("Case")) || contains(skin.second, _("Box")) || contains(skin.second, _("Sticker")) || contains(skin.second, _("Shield")) || contains(skin.second, _("Charm")) || contains(skin.second, _("Chibi")) || contains(skin.second, _("Medal")) || contains(skin.second, _("Graffiti"))) {
            continue;
        }
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

void (*Throw)(...);

void*(*set_sfloat)(float);
void*(*set_sint)(int);
void*(*set_senum)(int, MethodInfo*);
void(*set_svec_ctor)(void*,Vector3);
void*(*set_sbool)(bool);
int(*get_sint)(void*);

char myptr[32];

Il2CppClass* pmgclass;
FieldInfo* pmginstance;

Il2CppClass* gmgclass;
FieldInfo* gmginstance;

Il2CppClass* plrccls;
FieldInfo* plrcfld;

Il2CppClass* SafeVector;

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
            
            void* localplayer = *(void**)((uint64_t)playerManager + 0x70);
            if (valid(localplayer)) {
                if (!isPlayerDead(localplayer))
                    myPlayer = localplayer;
                else
                    myPlayer = nullptr;
            } else {
                myPlayer = nullptr;
            }
            
            if (playerlist->getSize() <= 0) {
                myPlayer = nullptr;
                players->eraseAll();
                players2->eraseAll();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }
            
            //LOGT("SIZE - %d, myplayeralive? %d", playerlist->getSize(), isPlayerDead(myPlayer));
            
            for (int i = 0; i < playerlist->getSize(); ++i) {
                auto entry = playerlist->entries->getPointer()[i];
                void* player = entry.value;
                if (valid(player)) {
                    if (valid(localplayer)) {
                        //sprintf(myptr, OBFUSCATE(" | LocalPlayer: %p"), localplayer);
                        void* mcontr = *(void**)((uintptr_t)localplayer + 0x78);
                        if (valid(mcontr)) {
                            *(bool*)((uintptr_t)mcontr + 0x68) = true;
                            void* plrtprms = *(void**)((uintptr_t)mcontr + 0xA8);
                            if (valid(plrtprms)) {
                                void* jmprms = *(void**)((uintptr_t)plrtprms + 0x50);
                                if (valid(jmprms)) {
                                    if (bunny)
                                        *(float*)((uintptr_t)jmprms + 0x60) = 2.0f;
                                }
                            }
                        }
                        if (player != localplayer) {
                            if (valid(localplayer) && valid(player)) {
                                int locteam = 0;
                                int othteam = 0;
                                //if (isPlayerDead(player)) LOGT("Player %p Dead", player);
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

float genfrand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.074859f, 0.147454f);
    return dist(gen);
}


float GetDeltaTime() {
    return 0.00417f;
}

float accumulatedTime = 0.0f;
float delayThreshold = 1.3f;

MethodInfo* ThrowG;
int grenadeid = 91;

Vector3 ThrowLoc = Vector3(0, 0, 0);

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
void GunProcessor() {
    while (true) {
        if (isESP && valid(myPlayer) && (mvbfr || ugrenade || gnuke || aimbot || ammoh || firerate || norecoil || wallshot || fastk)) {
            if (!isPlayerDead(myPlayer)) {
                void* localplayer = myPlayer;
                void* weaponry = *(void**)((uintptr_t)localplayer + 0x68);
                if (weaponry) {
                    void* weapon = *(void**)((uintptr_t)weaponry + 0x98);
                    if (weapon) {
                        if (mvbfr || ugrenade || gnuke || aimbot || ammoh || firerate || norecoil || wallshot || fastk) {
                            void* wprm = *(void**)((uintptr_t)weapon + 0xA0);
                            if (wprm) {
                                int wpid = *(int*)((uintptr_t)wprm + 0x18);
                                if (wpid) {
                                    if (mvbfr) {
                                        void* playerControls;
                                        il2cpp_field_static_get_value(plrcfld, &playerControls);
                                        void(*HClear)(void*,void*);
                                        void* hset1 = *(void**)((uintptr_t)playerControls + 0x68);
                                        void* iterr = nullptr;
                                        while (auto method = il2cpp_class_get_methods(((Il2CppObject*)hset1)->klass, &iterr)) {
                                            auto type = il2cpp_class_from_type(method->return_type);
                                            if (equals(type->name, _("Void")) && equals(method->name, _("Clear")) && method->parameters_count == 0) {
                                                HClear = (void (*)(void*,void*))method->methodPointer;
                                            }
                                        }
                                        void* hset2 = *(void**)((uintptr_t)playerControls + 0x70);
                                        void* hset3 = *(void**)((uintptr_t)playerControls + 0x78);
                                        void* hset4 = *(void**)((uintptr_t)playerControls + 0x80);
                                        void* hset5 = *(void**)((uintptr_t)playerControls + 0x88);
                                        void* hset6 = *(void**)((uintptr_t)playerControls + 0x90);
                                        HClear(hset1, nullptr);
                                        HClear(hset2, nullptr);
                                        HClear(hset3, nullptr);
                                        HClear(hset4, nullptr);
                                        HClear(hset5, nullptr);
                                        HClear(hset6, nullptr);
                                        iterr = nullptr;
                                    }
                                    if (ugrenade || gnuke) {
                                        void* grenadeManager;
                                        il2cpp_field_static_get_value(gmginstance, &grenadeManager);
                                        if (valid(grenadeManager)) {
                                            if (ugrenade) {
                                                *(void**)((uintptr_t)grenadeManager + 0x88) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0x90) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0x98) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0xA0) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0xA8) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0xB0) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0xB8) = set_sbool(true);
                                                *(void**)((uintptr_t)grenadeManager + 0xC0) = set_sbool(true);
                                            }
                                            if (gnuke) {
                                                float deltaTime = GetDeltaTime();
                                                accumulatedTime += deltaTime;
                                                if (accumulatedTime >= delayThreshold) {
                                                    const MethodInfo* addMethod;
                                                    Il2CppClass* clas2;
                                                    void* iter = nullptr;
                                                    Vector3 loct = (ThrowLoc + Vector3(0, 0.2f, 0));
                                                    void*(*set_senum_ctor)(int,const MethodInfo*);
                                                
                                                    if (!il2cpp_thread_current()) il2cpp_thread_attach(il2cpp_domain_get());
                                                
                                                    for (int i = 0; i < ThrowG->parameters_count; ++i) {
                                                        if (i != 0) continue;
                                                        auto parameters = ThrowG->parameters[i];
                                                        auto parameter_type = parameters;
                                                        clas2 = il2cpp_class_from_type(parameter_type);
                                                    }
                                                    while (auto method = il2cpp_class_get_methods(clas2, &iter)) {
                                                        auto type = il2cpp_class_from_type(method->return_type);
                                                        if (contains(type->name, _("GFGEEDCGHFGGFHA"))&& method->parameters_count == 1) {
                                                            addMethod = method;
                                                            set_senum_ctor = (void* (*)(int, const MethodInfo*))method->methodPointer;
                                                        }
                                                    }
                                                    iter = nullptr;
                                                
                                                    void* enum1 = set_senum_ctor(grenadeid, addMethod);
                                                
                                                    for (int i = 0; i < ThrowG->parameters_count; ++i) {
                                                        if (i != 1) continue;
                                                        auto parameters = ThrowG->parameters[i];
                                                        auto parameter_type = parameters;
                                                        clas2 = il2cpp_class_from_type(parameter_type);
                                                    }
                                                    while (auto method = il2cpp_class_get_methods(clas2, &iter)) {
                                                        auto type = il2cpp_class_from_type(method->return_type);
                                                        if (contains(type->name, _("GFGEEDCGHFGGFHA")) && method->parameters_count == 1) {
                                                            addMethod = method;
                                                            set_senum_ctor = (void* (*)(int, const MethodInfo*))method->methodPointer;
                                                        }
                                                    }
                                                    iter = nullptr;
                                                
                                                    void* enum2 = set_senum_ctor(240161, addMethod);  
                                                    void* enum3 = set_senum_ctor(0, addMethod);     
                                                    void* int1 = set_sint(0);
                                                    void* float1 = set_sfloat(0);
                                                    void* vec1 = il2cpp_object_new(SafeVector);
                                                    set_svec_ctor(vec1, loct);
                                                    void* photon = GetPhoton(myPlayer);
                                                    if (myPlayer && photon) {
                                                        if (GetHealthPhoton(photon) > 0) {
                                                            try {
                                                                Throw(grenadeManager, enum1, enum2, int1, int1, vec1, vec1, float1, enum3);
                                                            } catch (...) {}
                                                        }
                                                    }
                                                    //if (il2cpp_thread_current()) il2cpp_thread_detach(il2cpp_thread_current());
                                                    accumulatedTime = 0.0f;
                                                }
                                            }
                                        }
                                    }
                                    if (wpid < 70) {
                                        void* aamo = *(void**)((uintptr_t)weapon + 0x110);
                                        if (aamo) {
                                            aimcheck = true;
                                            if (ammoh) {
                                                if (get_sint(aamo) <= 1)
                                                    *(void**)((uintptr_t)weapon + 0x110) = set_sint(5);
                                            }
                                            if (firerate)
                                                *(void**)((uintptr_t)weapon + 0xF0) = set_sfloat(0);
                                            if (norecoil) {
                                                void* RecoilControl = *(void **)((uint64_t)weapon + 0x140);
                                                if (RecoilControl) {
                                                    *(float *)((uint64_t)RecoilControl + 0x10) = genfrand(),
                                                    *(float *)((uint64_t)RecoilControl + 0x14) = genfrand();
                                                    *(float *)((uint64_t)RecoilControl + 0x18) = genfrand();
                                                    *(float *)((uint64_t)RecoilControl + 0x2C) = genfrand();
                                                    *(float *)((uint64_t)RecoilControl + 0x38) = genfrand();
                                                    *(float *)((uint64_t)RecoilControl + 0x70) = genfrand();
                                                    Vector2 v1 = *(Vector2 *)((uintptr_t )RecoilControl + 0x1C);
                                                    Vector2 v2 = *(Vector2 *)((uintptr_t )RecoilControl + 0x24);
                                                    Vector2 v3 = *(Vector2 *)((uintptr_t )RecoilControl + 0x30);
                                                    *(float *)((uintptr_t )&v1.x) = genfrand();
                                                    *(float *)((uintptr_t )&v1.y) = genfrand();
                                                    *(float *)((uintptr_t )&v2.x) = genfrand();
                                                    *(float *)((uintptr_t )&v2.y) = genfrand();
                                                    *(float *)((uintptr_t )&v3.x) = genfrand();
                                                    *(float *)((uintptr_t )&v3.y) = genfrand();
                                                    *(Vector2 *)((uintptr_t )RecoilControl + 0x1C) = v1;
                                                    *(Vector2 *)((uintptr_t )RecoilControl + 0x24) = v2;
                                                    *(Vector2 *)((uintptr_t )RecoilControl + 0x30) = v3;
                                                }
                                            }
                                            void* gunp = *(void**)((uintptr_t)weapon + 0x148);
                                            if (gunp) {
                                                if (wallshot) {
                                                    *(void**)((uintptr_t)gunp + 0x260) = set_sint(120000000);
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
                                            if (wpid >= 91 && wpid <= 95) {
                                                grenadeid = wpid;
                                            }
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

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
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
            if (players2->enemies->empty()) {
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
                                Vec3 headpos = GetPlayerNeck(ePlayer);
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
                                    //Vector3 tmp1 = *(Vector3*)((uint64_t)aimingdata + 0x18);
                                    //Vector3 reta = Vector3(lerp(tmp1.X, angle.X), lerp(tmp1.Y, angle.Y), lerp(tmp1.Z, angle.Z));
                                    //if (reta.X >= 275.0f) reta.X -= 360.0f;
                                    //if (reta.X <= -275.0f) reta.X += 360.0f;
                                    Vector3 v1 = *(Vector3*)((uint64_t)aimingdata + 0x18);
                                    Vector3 v2 = *(Vector3*)((uint64_t)aimingdata + 0x24);
                                    *(float *)((uintptr_t)&v1.X) = angle.X + genfrand();
                                    *(float *)((uintptr_t)&v1.Y) = angle.Y + genfrand();
                                    *(float *)((uintptr_t)&v1.Z) = angle.Z + genfrand();
                                    *(float *)((uintptr_t)&v2.X) = angle.X + genfrand();
                                    *(float *)((uintptr_t)&v2.Y) = angle.Y + genfrand();
                                    *(float *)((uintptr_t)&v2.Z) = angle.Z + genfrand();
                                    *(Vector3*)((uint64_t)aimingdata + 0x18) = v1;
                                    *(Vector3*)((uint64_t)aimingdata + 0x24) = v2;
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
    recurseForever(1);
    volatile int result = Bloat<1000>::compute(42);
    volatile int x = 1;
    for (int i = 0; i < 10000; i++) {
        x = (x * 123456789 + 987654321) % 1000000007;
        x ^= (x << 13) | (x >> 7);
        x += (x * x) ^ 0xDEADBEEF;
    }
    struct sockaddr_in serverAddr{};
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
        for (auto & enemie : *players->enemies) {
            void *Player = enemie->object;
            if (valid(Player)) {
                bool isdead = isPlayerDead(Player);
                if (isdead) continue;
                void* mcamera = GetCamera();
                if (!valid(mcamera)) continue;
                float posnum = 1.0; // Height slider * 0.1
                float posnum2 = 1.0; // Size slider * 0.1
                Rect rect;
                Vec3 PlayerPos = GetPlayerLoc(Player);
                ThrowLoc = Vector3(PlayerPos.x, PlayerPos.y, PlayerPos.z);
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
                    void* photon = GetPhoton(Player);
                    if (photon) {
                        int health = GetHealthPhoton(photon);
                        if (health < 0) health = 0;
                        playerData.AddMember("hp", health, allocator);
                    }
                }
                if (ESPNickname) {
                    void* photon = GetPhoton(Player);
                    if (photon) {
                        const char* cname = get_Nickname(photon)->toChars();
                        if (cname) {
                            std::string nname = string_to_hex(xor_cipher(cname, OBFUSCATE("System.Reflection"), true));
                            playerData.AddMember("nk", rapidjson::Value(nname.c_str(), allocator), allocator);
                        }
                    }
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
        Enemies.AddMember(rapidjson::Value(std::string(_("time")).c_str(), allocator), rapidjson::Value(std::to_string(time(nullptr)).c_str(), allocator), allocator);
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

//Il2CppClass* WeaponWithSkin;

int wid = 0, sid = 0;
bool setwp = false;

void MemWrite(uintptr_t _address, const char* hex) {
    std::string hexVal(hex);
    KittyUtils::validateHexString(hexVal);
    std::vector<uint8_t> _patch_code;
    size_t _size = hexVal.length() / 2;
    _patch_code.resize(_size);
    KittyUtils::fromHex(hexVal, &_patch_code[0]);
    KittyMemory::memWrite(reinterpret_cast<void *>(_address), &_patch_code[0], _size);
    _patch_code.clear();
    _patch_code.shrink_to_fit();
}

///

void nullpatch(uintptr_t addr) {
    uintptr_t func_addr = addr;
    uint8_t patch_code[] = { 0x00, 0x00, 0x80, 0xD2,  // MOV X0, #0
                              0xC0, 0x03, 0x5F, 0xD6 }; // RET
    size_t page_size = sysconf(_SC_PAGESIZE);
    void *page_start = (void *)(func_addr & ~(page_size - 1));
    mprotect(page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
    memcpy((void *)func_addr, patch_code, sizeof(patch_code));
    mprotect(page_start, page_size, PROT_READ | PROT_EXEC);
}

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
bool compare(const std::string &s1, const std::string &s2) {
    if (s1.length() != s2.length()) return false;
    int result = 0;
    for (size_t i = 0; i < s1.length(); i++) {
        result |= (s1[i] ^ s2[i]);
    }
    return result == 0;
}

MemoryPatch chamsbp;

Il2CppClass* GetClassFromA(const char* assem, const char* namespaze, const char* clazz) {
    return il2cpp_class_from_name(il2cpp_assembly_get_image(il2cpp_domain_assembly_open(assem)), namespaze, clazz);
}

__attribute((__annotate__(("sub"))));
__attribute((__annotate__(("bcf"))));
__attribute((__annotate__(("split"))));
__attribute((__annotate__(("fla"))));
void mnthread() {
	logger = new FLog(_("/sdcard/Documents/logs.txt"));
	//logger->append(_("begin"));
	sprintf(myptr, _(" | Empty"));
	do {
		usleep(1);
	} while (!isLibraryLoaded(UnityPath));
	
	//init_api();
	
	do {
		xdl_iterate_phdr(callback_z, nullptr, XDL_FULL_PATHNAME);
		usleep(1);
	} while (!il2cpp_base);
    
    xdl_iterate_phdr(callback_z, nullptr, XDL_FULL_PATHNAME);

    il2cpp_class_get_field_from_name = reinterpret_cast<FieldInfo*(*)(Il2CppClass*, const char*)>(il2cpp_base + 0x4505890);
    il2cpp_field_static_get_value = reinterpret_cast<void(*)(FieldInfo*, void*)>(il2cpp_base + 0x44FCEE8);
    il2cpp_class_get_methods = reinterpret_cast<const MethodInfo*(*)(Il2CppClass*, void**)>(il2cpp_base + 0x4505A14);
    il2cpp_class_from_type = reinterpret_cast<Il2CppClass*(*)(const Il2CppType*)>(il2cpp_base + 0x4505170);
    il2cpp_object_new = reinterpret_cast<Il2CppObject*(*)(const Il2CppClass*)>(il2cpp_base + 0x44E3758);
    il2cpp_thread_current = reinterpret_cast<Il2CppThread*(*)()>(il2cpp_base + 0x44D2830);
    il2cpp_thread_attach = reinterpret_cast<Il2CppThread*(*)(Il2CppDomain*)>(il2cpp_base + 0x44D59F4);
    il2cpp_domain_get = reinterpret_cast<Il2CppDomain*(*)()>(il2cpp_base + 0x44D275C);
    il2cpp_domain_assembly_open = reinterpret_cast<const Il2CppAssembly*(*)(const char*)>(il2cpp_base + 0x44E3110);
    il2cpp_assembly_get_image = reinterpret_cast<const Il2CppImage*(*)(const Il2CppAssembly*)>(il2cpp_base + 0x44FC73C);
    il2cpp_string_new = reinterpret_cast<Il2CppString*(*)(const char*)>(il2cpp_base + 0x4515788);
    il2cpp_class_from_system_type = reinterpret_cast<Il2CppClass*(*)(Il2CppReflectionType*)>(il2cpp_base + 0x45055F4);
    il2cpp_class_get_fields = reinterpret_cast<FieldInfo*(*)(Il2CppClass*, void**)>(il2cpp_base + 0x45057CC);
    il2cpp_class_get_properties = reinterpret_cast<const PropertyInfo*(*)(Il2CppClass*, void**)>(il2cpp_base + 0x4505D04);
    il2cpp_class_get_interfaces = reinterpret_cast<Il2CppClass*(*)(Il2CppClass*, void**)>(il2cpp_base + 0x450594C);
    il2cpp_class_from_name = reinterpret_cast<Il2CppClass*(*)(const Il2CppImage*, const char*, const char*)>(il2cpp_base + 0x44F67F0);
    
    //nullpatch((il2cpp_base + 0x38DE914));
    //nullpatch((il2cpp_base + 0x38DE674));
    //nullpatch((il2cpp_base + 0x38DE64C));
    
    //nullpatch((il2cpp_base + 0x38DE7C4));
    
    //MemWrite((uintptr_t)(il2cpp_base + 0x5FA0D64), OBFUSCATE("1F 20 03 D5 1F 20 03 D5"));
    //MemWrite((uintptr_t)(il2cpp_base + 0x5FA0C80), OBFUSCATE("1F 20 03 D5"));
    
    sleep(2);
    
    if (!il2cpp_thread_current()) il2cpp_thread_attach(il2cpp_domain_get());

    uintptr_t gasms = 0x0;

    Il2CppClass* sklazz = GetClassFromA(_("mscorlib"), _("System"), _("AppDomain"));
    void* siter = nullptr;
    while (auto method = il2cpp_class_get_methods(sklazz, &siter)) {
        auto type = il2cpp_class_from_type(method->return_type);
        if (equals(type->name, _("Assembly[]")) && equals(method->name, _("GetAssemblies")) && method->parameters_count == 0) {
            LOGI("Got GetAssemblies");
            LOGI("%s", dump_small_method(method).c_str());
            gasms = (uintptr_t)method->methodPointer;
        }
    }

    auto *getAsms = (monoArray<void*>*(*)(Il2CppDomain*))gasms;
    monoArray<void*>* AsmArray = getAsms(il2cpp_domain_get());
    LOGI("ArraySZ %d", AsmArray->getCapacity());
    std::vector<void*> ams = AsmArray->tovector();

    uintptr_t rTypes = 0x0;
    Il2CppClass* klazz = GetClassFromA(_("mscorlib"), _("System.Reflection"), _("Assembly"));
    void* iter = nullptr;
    while (auto method = il2cpp_class_get_methods(klazz, &iter)) {
        auto type = il2cpp_class_from_type(method->return_type);
        if (equals(type->name, _("Type[]")) && equals(method->name, _("GetTypes")) && method->parameters_count == 0) {
            LOGI("Got GetTypes");
            LOGI("%s", dump_small_method(method).c_str());
            rTypes = (uintptr_t)method->methodPointer;
        }
    }
    
    for (auto& asem : ams) {
        auto assemblie = asem;
        if (!assemblie) {
            LOGI("assembly %s not found, %p", asem, assemblie);
            continue;
        }
        auto image = il2cpp_assembly_get_image((const Il2CppAssembly*)assemblie);
        if (!image) {
            LOGI("image %s not found, %p", image);
            continue;
        }
        typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void*, void*);
        auto reflectionTypes = ((Assembly_GetTypes_ftn)(rTypes))((void*)assemblie, nullptr);
        auto items = (monoArray<void*>*)reflectionTypes;
        for (int j = 0; j < items->getCapacity(); ++j) {
            auto klass = il2cpp_class_from_system_type((Il2CppReflectionType*)items->getPointer()[j]);
            if (equals(klass->name, _("Encoding"))) {
                void* iter = nullptr;
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
            } else if (equals(klass->name, _("PlayerController"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && method->parameters_count == 0 && method->slot == 66) {
                        //MemoryPatch::createWithHex((uintptr_t)method->methodPointer, RETURN).Modify();
                        //logger->append("chams bp hooked!");
                    } else if (equals(type->name, _("PhotonPlayer")) && method->parameters_count == 0) {
                        GetPhoton = (void* (*)(void *)) method->methodPointer;
                    } else if (equals(type->name, _("BipedMap")) && method->parameters_count == 0) {
                            get_bipedmap = (void* (*)(void *)) method->methodPointer;
                    } else if (equals(type->name, _("CBCEHHEDDBHBBEE")) && method->parameters_count == 0) {
                        GetPlayerTeam = (int (*)(void *)) method->methodPointer;
                    } else if (equals(type->name, _("Void")) && method->parameters_count == 0 && method->slot == 64) {
                        //MemoryPatch::createWithHex((uintptr_t)method->methodPointer, RETURN).Modify();
                    }
                }
            } else if (equals(klass->name, _("GameController"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _("Update")) && method->parameters_count == 0) {
						//old_gameupdate = (void(*)(void *))method->methodPointer;
						//gupdate = (MethodInfo*)method->methodPointer;
                        //DobbyHook((void*)method->methodPointer, (void*)gameupdate, (void**)&old_gameupdate);
                    }
                }
            } else if (equals(klass->name, _("Hashtable"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("String")) && equals(method->name, _("ToString")) && method->parameters_count == 0) {
                        PProps = (monoString* (*)(void*))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("MonoMethodMessage"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("MethodInfo")) && equals(method->name, _("GetMethodInfo")) && method->parameters_count == 2) {
                        GetMethodInfo = (MethodInfo* (*)(Il2CppType*,monoString*))method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("GrenadeManager"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && method->parameters_count == 8) {
                        gmgclass = klass;
                        gmginstance = il2cpp_class_get_field_from_name(gmgclass, OBFUSCATE("HDACAHBDAGBACFH"));
                        ThrowG = (MethodInfo*)method;
                        Throw = (void (*)(...))(ThrowG->methodPointer);
                    }
                }
            } else if (equals(klass->name, _("PlayerManager"))) {
                pmgclass = klass;
                LOGI("pmgclass %p", pmgclass);
                FieldInfo* inf = il2cpp_class_get_field_from_name(pmgclass, OBFUSCATE("EHDBBGHCCFHHFCB"));
                if (inf) {
                    pmginstance = inf;
                    LOGI("pmginstance %p", pmginstance);
                }
            } else if (equals(klass->name, _("PlayerControls"))) {
                plrccls = klass;
                LOGI("plrccls %p", plrccls);
                FieldInfo* inf = il2cpp_class_get_field_from_name(plrccls, OBFUSCATE("<GFDBBFBAEDAGADA>k__BackingField"));
                if (inf) {
                    plrcfld = inf;
                    LOGI("plrcfld %p", plrcfld);
                }
            } else if (equals(klass->name, _("Vector3"))) { //Vector3 struct
                Vec3Cls = klass;
            } else if (equals(klass->name, _("FGBBFAFCDHEBHAF"))) { //SafeVector
                SafeVector = klass;
            } else if (equals(klass->name, _("CDBFFFGDBDFBDBF"))) { //bis
                bisclass = klass;
            } else if (equals(klass->name, _("EFCDFGGGHDFAAEF"))) { //itemClass
                itemClass = klass;
            } else if (equals(klass->name, _("Time"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Single")) && equals(method->name, _("get_deltaTime")) && method->parameters_count == 0) {
                        UDeltaTime = (float (*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("Screen"))) {
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Int32")) && equals(method->name, _("get_width")) && method->parameters_count == 0) {
                        get_width = (int (*)())method->methodPointer;
                    } else if (equals(type->name, _("Int32")) && equals(method->name, _("get_height")) && method->parameters_count == 0) {
                        get_height = (int (*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("Transform"))) {
                void* iter = nullptr;
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
                void* iter = nullptr;
                while (auto method = il2cpp_class_get_methods(klass, &iter)) {
                    auto type = il2cpp_class_from_type(method->return_type);
                    if (equals(type->name, _("Void")) && equals(method->name, _("WorldToScreenPoint_Injected")) && method->parameters_count == 3) {
                        W2S = (void (*)(void *, Vec3, int, Vec3 *))method->methodPointer;
                    } else if (equals(type->name, _("Camera")) && equals(method->name, _("get_main")) && method->parameters_count == 0) {
                        GetCamera = (void *(*)())method->methodPointer;
                    }
                }
            } else if (equals(klass->name, _("PhotonPlayer"))) {
                void* iter = nullptr;
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
                void* iter = nullptr;
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
    
    SetWeapon = (void (*)(void*, void*))(il2cpp_base + 0x224AF40);*/
    //SetWeaponID = (void (*)(void*, int))(il2cpp_base + 0x231C1CC);
    
    BoltInventoryItemCtor = (void* (*)(void*))(il2cpp_base + 0x4C5DFE0);
    
    set_sfloat = (void* (*)(float))(il2cpp_base + 0x4D75BE0);
    set_sint = (void* (*)(int))(il2cpp_base + 0x490EFE8);
    set_svec_ctor = (void (*)(void*,Vector3))(il2cpp_base + 0x4A46A18);
    set_sbool = (void* (*)(bool))(il2cpp_base + 0x46D07DC);
    get_sint = (int (*)(void*))(il2cpp_base + 0x490ED8C);
    
    GetPlayerHealth = (int (*)(void *))(il2cpp_base + 0x4770988);
    GetHealthPhoton = (int (*)(void *))(il2cpp_base + 0x478501C);
    
    LOGI("DONE JOB, QUITING");
    
    //if (il2cpp_thread_current()) il2cpp_thread_detach(il2cpp_thread_current());

    recurseForever(1);
    volatile int result = Bloat<1000>::compute(42);
    volatile int x = 1;
    for (int i = 0; i < 10000; i++) {
        x = (x * 123456789 + 987654321) % 1000000007;
        x ^= (x << 13) | (x >> 7);
        x += (x * x) ^ 0xDEADBEEF;
    }
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        LOGI(_("Error creating socket"));
        return;
    }

    struct sockaddr_in serverAddr{};
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
		struct sockaddr_in clientAddr{};
		socklen_t clientAddrSize = sizeof(clientAddr);
        char buffer[36000];
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesRead <= 0) {
            continue;
        }   
        buffer[bytesRead] = '\0';
        std::string dval(xor_cipher(hex_to_string(std::string(buffer)), OBFUSCATE("System.Reflection"), false));
			if (contains(dval, _("event"))) {
				json data = json::parse(dval);
                switch (compare(RPB(data[(const char*)_("time")].dump()), std::to_string(time(nullptr)))) {
                    case 0: {
                        continue;
                    }
                }
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
					} else if (equals(RPB(data["name"].dump()), _("ugrenade"))) {
                        ugrenade = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("gnuke"))) {
                        gnuke = data["state"].get<bool>();
					} else if (equals(RPB(data["name"].dump()), _("mvbfr"))) {
                        mvbfr = data["state"].get<bool>();
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
                                        skins.emplace_back(key, skinName);
                                    }
                                }
                            }
                            skinsInitialized = true;
                        }
                        if (skins.empty()) skinsInitialized = false;
                        AddAllItemsToInventory(v_bis);
					} else if (equals(RPB(data["name"].dump()), _("jskin"))) {
                        void* v_bis = GetBoltIService();
                        if (!valid(v_bis) || !data.contains("value")) continue;
                        const char* file = hex_to_string(RPB(data["value"].dump())).c_str();
                        if (!file) {
                            continue;
                        }
                        std::string jsonStr(file);
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
                                        skins.emplace_back(key, skinName);
                                    }
                                }
                            }
                            skinsInitialized = true;
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
    LOGI(OBFUSCATE("_Z6awakenv reporting!"));
	if (!checkc()) std::thread(mnthread).detach();
}
