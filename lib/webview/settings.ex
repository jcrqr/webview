defmodule WebView.Settings do
  @moduledoc """
  Settings to configure the `WebView`.

  The `WebView.Settings` structure has the following attribtues:

  * `title` - string. Default is `"WebView"`
  * `url` - string. Default is `"https://elixir-lang.org"`
  * `size` - tuple. Default is `{800, 600}`
  * `resizable` - bool. Makes the window resizable. Default is `true`
  * `debug` - bool. Enables debugging (developer tools). Default is `true`
  * `blocking` - bool. Makes the loop blocking, not recommended. Default is `false`
  """

  defstruct title: "WebView",
            url: "https://elixir-lang.org",
            size: {800, 600},
            resizable: true,
            debug: true,
            blocking?: false
end
