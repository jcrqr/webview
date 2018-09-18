# WebView

[Installation](#installation) | [Documentation](#usage) | [License](#license)

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

For usage information, please check the [examples](https://github.com/und0ck3d/webview/blob/master/examples)
provided for now.

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
