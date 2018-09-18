# WebView

[Installation](#installation) | [Usage](#usage) | [Documentation](#documentation) | [License](#license)

> Build cross platform desktop apps with Elixir and web technologies.

This library provides Elixir bindings for the [webview](https://github.com/zserge/webview)
library to allow easy creation of cross platform Elixir desktop apps with GUIs
based on web technologies.

The [webview](https://github.com/zserge/webview) library uses `Cocoa/WebKit` on
macOS, `gtk-webkit2` on Linux and `MSHTML (IE10/11)` on Windows, making this a good
alternative to [Electron](https://electronjs.org/).

Check the [NIF project](https://github.com/und0ck3d/webview/projects/1) to track
the implementation progress.

It's recommended to also read the [webview readme](https://github.com/Boscop/web-view/blob/master/README.md)
for a better understanding of how the library works behind the scenes. You'll also
find a list of bindings in other languages there.

**Disclaimer:** this is work in progress (early development) and breaking changes
will most probably occur without notice, currently **not** production-ready.

## Installation

WebView can be installed by adding `webview` to your list of dependencies in `mix.exs`:

```elixir
def deps do
  [
    {:webview, github: "und0ck3d/webview"}
  ]
end
```

Will be published on [Hex](https://hex.pm) when `0.1.0` is released.

## Usage

**Starting WebView**

```console
iex> opts = [title: "Hello, WebView!", url: "https://elixir-lang.org"]
iex> WebView.start(opts)
```

Alternatively you may add `WebView` to a supervision tree:

```elixir
webview_config = [title: "Hello, WebView!", url: "https://elixir-lang.org"]

children = [
  {WebView, webview_config}
]
```

**Manipulating WebView**

(after having started `WebView`)

Set the title:

```elixir
WebView.set_title("Elixir is awesome!")
```

Set fullscreen:

```elixir
WebView.set_fullscreen(true) # Turns fullscreen on
WebView.set_fullscreen(false) # Turns fullscreen off
```

Evaluate JavaScript code:

```elixir
WebView.eval("console.log('Hi, from Elixir!')")
```

Injectg CSS styles:

```elixir
WebView.inject_css("body { background: #000; color: #fff; }")
```

Currently it isn't possible to update neither the page's URL nor the HTML directly
using [zserge/webview](https://github.com/zserge/webview) because the library
doesn't have support for it. Those and other additional features will be added
to WebView in the future to easily allow different use cases (such as update the
HTML directly, implement navigation on back-end, use a web server such as Phoenix,
SPAs and more).

For more examples, please check the [examples](https://github.com/und0ck3d/webview/blob/master/examples) directory.

## Documentation

Documentation isn't available online. You can generate it locally with:

```console
$ mix docs
```

## Contributing

Contributions are very welcome, specially improvements to the
[NIF](https://github.com/und0ck3d/webview/blob/master/lib/webview/native.ex) and
the [Makefile](https://github.com/und0ck3d/webview/blob/master/Makefile) to support
cross platform.

For now, simply fork this repository and make the changes, open an issue and
create a pull request. Don't forget to run `mix format` before committing, please.

## License

Both [und0ck3d/webview](https://github.com/und0ck3d/webview) and
[zserge/webview](https://github.com/zserge/webview) projects are released under
the [MIT license](https://github.com/und0ck3d/webview/blob/master/LICENSE).
