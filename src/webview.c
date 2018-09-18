#include <assert.h>
#include "erl_nif.h"
#define WEBVIEW_IMPLEMENTATION
#include "webview.h"

// state_t
// A structure to hold the webview state.
typedef struct
{
    struct webview*     w;
    ERL_NIF_TERM        atom_ok;
    ERL_NIF_TERM        atom_error;
    ERL_NIF_TERM        atom_stop;
} state_t;

// load
// Loads the NIF.
static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    state_t* state = (state_t*) enif_alloc(sizeof(state_t));
    if(state == NULL) return -1;

    state->w = (struct webview*) enif_alloc(sizeof(struct webview));
    state->atom_ok = enif_make_atom(env, "ok");
    state->atom_error = enif_make_atom(env, "error");
    state->atom_stop = enif_make_atom(env, "stop");

    *priv = (void*) state;

    return 0;
}

// unload
// Unloads the NIF.
static void
unload(ErlNifEnv* env, void* priv)
{
    state_t* state = (state_t*) priv;

    enif_free(state->w);
    enif_free(state);
}

// create
// Creates a new webview.
// Implements the `webview_init` function.
static ERL_NIF_TERM
create(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);

    // get arguments
    ErlNifBinary title_bin, url_bin;
    int resizable, debug, width, height;

    if (enif_inspect_binary(env, argv[0], &title_bin) == 0) {
        return enif_make_badarg(env);
    }

    if (enif_inspect_binary(env, argv[1], &url_bin) == 0) {
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

    // make local copy of title and url
    const char *title = strndup((char*) title_bin.data, title_bin.size);
    const char *url = strndup((char*) url_bin.data, url_bin.size);

    // setup webview
    state->w->title = title;
    state->w->url = url;
    state->w->width = width;
    state->w->height = height;
    state->w->resizable = resizable;
    state->w->debug = debug;

    webview_debug("title: %s", state->w->title);
    webview_debug("url: %s", state->w->url);

    // init the webview
    if (webview_init(state->w) != 0) {
        enif_free(state->w);

        return state->atom_error;
    }

    // free binaries
    enif_release_binary(&title_bin);
    enif_release_binary(&url_bin);

    // free local copies
    free(title);
    free(url);

    return state->atom_ok;
}

// loop
// Calls the webview loop. Accepts a `blocking` argument
// that should be either `0` or `1` for non-blocking and blocking loop, respectively.
// Implements the `webview_loop` function.
static ERL_NIF_TERM
loop(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);

    // get argument
    int blocking;

    if (enif_get_int(env, argv[0], &blocking) == 0) {
        return enif_make_badarg(env);
    }

    // loop
    if (webview_loop(state->w, blocking) == 0) {
      return state->atom_ok;
    }
    else {
      return state->atom_stop;
    }
}

// set_title
// Sets the webview title. Accepts a `title` argument that must be an Erlang binary.
// Implements the `webview_loop` function.
static ERL_NIF_TERM
set_title(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);

    // get argument
    ErlNifBinary title_bin;

    if (enif_inspect_binary(env, argv[0], &title_bin) == 0) {
        return enif_make_badarg(env);
    }

    // make local copy of title
    const char *title = strndup((char*) title_bin.data, title_bin.size);

    // set the title
    webview_set_title(state->w, title);

    // release the title binary
    enif_release_binary(&title_bin);

    // free local copy
    free(title);

    return state->atom_ok;
}

// set_fullscreen
// Toggles the webview fullscreen property. Accepts a `fullscreen` argument
// that should be either `0` or `1` to turn fullscreen off and on respectively.
// Implements the `webview_set_fullscreen` function.
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

// eval
// Evaluates JavaScript code in the webview. Accepts a `js` argument that
// should be an Erlang binary with the code to be evaluated.
// Implements `webview_eval` function.
// TODO: this function should return the result of `webview_eval`.
static ERL_NIF_TERM
eval(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);

    // parse argument
    ErlNifBinary js_bin;

    if (enif_inspect_binary(env, argv[0], &js_bin) == 0) {
        return enif_make_badarg(env);
    }

    // evaluate JS
    webview_eval(state->w, (const char*) js_bin.data);

    // release the binary
    enif_release_binary(&js_bin);

    return state->atom_ok;
}

// inject_css
// Injects CSS code in the webview. Accepts a `css` argument that should be an
// Erlang binary with the styles to be injected.
// Implements `webview_inject_css` function.
// TODO: this function should return the result of `webview_inject_css`.
static ERL_NIF_TERM
inject_css(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    ErlNifBinary css_bin;

    if (enif_inspect_binary(env, argv[0], &css_bin) == 0) {
        return enif_make_badarg(env);
    }

    webview_inject_css(state->w, (const char*) css_bin.data);

    enif_release_binary(&css_bin);

    return state->atom_ok;
}

// Initialize the NIF
static ErlNifFunc nif_funcs[] = {
    {"create", 6, create, 0},
    {"loop", 1, loop, 0},
    {"set_title", 1, set_title, 0},
    {"set_fullscreen", 1, set_fullscreen, 0},
    {"eval", 1, eval, 0},
    {"inject_css", 1, inject_css, 0},
};

ERL_NIF_INIT(Elixir.WebView.Native, nif_funcs, &load, NULL, NULL, &unload)
