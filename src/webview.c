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
} state_t;

// load
// Loads the NIF.
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

// unload
// Unloads the NIF.
static void
unload(ErlNifEnv* env, void* priv)
{
    state_t* state = (state_t*) priv;

    enif_free(state);
}

// create
// Creates a new webview.
// Implements the `webview_init` function.
static ERL_NIF_TERM
create(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);
    struct webview *w = (struct webview *) calloc(1, sizeof(*w));

    // get arguments
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

    // setup webview
    w->title = title.data;
    w->url = url.data;
    w->width = width;
    w->height = height;
    w->resizable = resizable;
    w->debug = debug;

    // release unecessary binaries
    enif_release_binary(&title);
    enif_release_binary(&url);

    // init the webview
    if (webview_init(w) != 0) {
        enif_free(w);

        return state->atom_error;
    }

    state->w = w;

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

    // parse argument
    int blocking;

    if (enif_get_int(env, argv[0], &blocking) == 0) {
        return enif_make_badarg(env);
    }

    // loop
    webview_loop(state->w, blocking);

    return state->atom_ok;
}

// set_title
// Sets the webview title. Accepts a `title` argument that must be an Erlang binary.
// Implements the `webview_loop` function.
// TODO: this function should return the result of `webview_loop`.
static ERL_NIF_TERM
set_title(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    state_t* state = (state_t*) enif_priv_data(env);

    // parse argument
    ErlNifBinary title_bin;

    if (enif_inspect_binary(env, argv[0], &title_bin) == 0) {
        return enif_make_badarg(env);
    }

    // set the title
    webview_set_title(state->w, title_bin.data);

    // release the title binary
    enif_release_binary(&title_bin);

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
    webview_eval(state->w, js_bin.data);

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

    webview_inject_css(state->w, css_bin.data);

    enif_release_binary(&css_bin);

    return state->atom_ok;
}

// Initialize the NIF
static ErlNifFunc nif_funcs[] = {
    {"create", 6, create},
    {"loop", 1, loop},
    {"set_title", 1, set_title},
    {"set_fullscreen", 1, set_fullscreen},
    {"eval", 1, eval},
    {"inject_css", 1, inject_css},
};

ERL_NIF_INIT(Elixir.WebView.Native, nif_funcs, &load, NULL, NULL, &unload)
