#include <assert.h>
#include "erl_nif.h"
#define WEBVIEW_IMPLEMENTATION
#include "webview.h"

typedef struct
{
    struct webview*            w;
    ERL_NIF_TERM        atom_ok;
    ERL_NIF_TERM        atom_error;
} state_t;

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    state_t* state = (state_t*) enif_alloc(sizeof(state_t));
    if(state == NULL) return -1;

    state->atom_ok = enif_make_atom(env, "ok");
    state->atom_error = enif_make_atom(env, "error");

    *priv = (void*) state;

    return 0;
}

static void
unload(ErlNifEnv* env, void* priv)
{
    state_t* state = (state_t*) priv;

    enif_free(state);
}

static ERL_NIF_TERM
create(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
  	struct webview *w = (struct webview *) calloc(1, sizeof(*w));
    ErlNifBinary title, url;
    int resizable, debug, width, height;

    if (enif_inspect_binary(env, argv[0], &title) == 0) {
      return enif_make_badarg(env);
    }

    if (enif_inspect_binary(env, argv[1], &url) == 0) {
      return enif_make_badarg(env);
    }

    if (enif_get_int(env, argv[2], &width) == 0) {
      return enif_make_badarg(env);
    }

    if (enif_get_int(env, argv[3], &height) == 0) {
      return enif_make_badarg(env);
    }

    if (enif_get_int(env, argv[4], &resizable) == 0) {
      return enif_make_badarg(env);
    }

    if (enif_get_int(env, argv[5], &debug) == 0) {
      return enif_make_badarg(env);
    }

    w->title = title.data;
    w->url = url.data;
    w->width = width;
    w->height = height;
    w->resizable = resizable;
    w->debug = debug;

    enif_release_binary(&title);
    enif_release_binary(&url);

    if (webview_init(w) != 0) {
      enif_free(w);

      return state->atom_error;
    }

    state->w = w;

    return state->atom_ok;
}

static ERL_NIF_TERM
loop(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    int blocking;

    if (enif_get_int(env, argv[0], &blocking) == 0) {
      return enif_make_badarg(env);
    }

    webview_loop(state->w, blocking);

    return state->atom_ok;
}

static ERL_NIF_TERM
set_title(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    ErlNifBinary title_bin;

    if (enif_inspect_binary(env, argv[0], &title_bin) == 0) {
      return enif_make_badarg(env);
    }

    webview_set_title(state->w, title_bin.data);

    enif_release_binary(&title_bin);

    return state->atom_ok;
}

static ERL_NIF_TERM
set_fullscreen(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    int fullscreen;

    if (enif_get_int(env, argv[0], &fullscreen) == 0) {
      return enif_make_badarg(env);
    }

    webview_set_fullscreen(state->w, fullscreen);

    return state->atom_ok;
}

static ERL_NIF_TERM
eval(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    ErlNifBinary js_bin;

    if (enif_inspect_binary(env, argv[0], &js_bin) == 0) {
      return enif_make_badarg(env);
    }

    webview_eval(state->w, js_bin.data);

    enif_release_binary(&js_bin);

    return state->atom_ok;
}

static ERL_NIF_TERM
inject_css(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    ErlNifBinary css_bin;

    if (enif_inspect_binary(env, argv[0], &css_bin) == 0) {
      return enif_make_badarg(env);
    }

    webview_inject_css(state->w, css_bin.data);

    enif_release_binary(&css_bin);

    return state->atom_ok;
}

static ErlNifFunc nif_funcs[] = {
    {"create", 6, create},
    {"loop", 1, loop},
    {"set_title", 1, set_title},
    {"set_fullscreen", 1, set_fullscreen},
    {"eval", 1, eval},
    {"inject_css", 1, inject_css},
};

ERL_NIF_INIT(Elixir.WebView.Native, nif_funcs, &load, NULL, NULL, &unload)
