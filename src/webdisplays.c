#include <stdio.h>
#include <stdbool.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef void (*WDCallback)(const char *json);

static WDCallback globalCallback = NULL;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
void wd_handle_response(const char *json) {
    if (globalCallback) {
        globalCallback(json);
    }
}

static void wdExecRequest(const char *cmd, WDCallback callback) {
    globalCallback = callback;

    char jsCmd[512];
    snprintf(jsCmd, sizeof(jsCmd),
             "window.mcefQuery({"
             "request: 'WebDisplays_%s',"
             "persistent: true,"
             "onSuccess: function(resp) { Module._wd_handle_response(UTF8ToString(resp)); },"
             "onFailure: function(code, msg) { console.error(msg); }"
             "});",
             cmd);

    emscripten_run_script(jsCmd);
}
#else
static void wdExecRequest(const char *cmd, WDCallback callback) {}
#endif

void wdGetSize(WDCallback callback) {
    wdExecRequest("GetSize", callback);
}

void wdGetUpgrades(WDCallback callback) {
    wdExecRequest("GetUpgrades", callback);
}

void wdIsOwner(WDCallback callback) {
    wdExecRequest("IsOwner", callback);
}

void wdGetRotation(WDCallback callback) {
    wdExecRequest("GetRotation", callback);
}

void wdGetSide(WDCallback callback) {
    wdExecRequest("GetSide", callback);
}

void wdGetLocation(WDCallback callback) {
    wdExecRequest("GetLocation", callback);
}

void wdGetRedstoneAt(int x, int y, WDCallback callback) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "GetRedstoneAt(%d,%d)", x, y);
    wdExecRequest(cmd, callback);
}

void wdGetRedstoneArray(WDCallback callback) {
    wdExecRequest("GetRedstoneArray", callback);
}

void wdClearRedstone(WDCallback callback) {
    wdExecRequest("ClearRedstone", callback);
}

void wdSetRedstoneAt(int x, int y, bool state, WDCallback callback) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "SetRedstoneAt(%d,%d,%d)", x, y, state ? 1 : 0);
    wdExecRequest(cmd, callback);
}

void wdIsEmitting(int x, int y, WDCallback callback) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "IsEmitting(%d,%d)", x, y);
    wdExecRequest(cmd, callback);
}

void wdGetEmissionArray(WDCallback callback) {
    wdExecRequest("GetEmissionArray", callback);
}
