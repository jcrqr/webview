defmodule Minimal.Application do
  @moduledoc false

  use Application

  def start(_type, _args) do
    webview_config = Application.get_env(:minimal, :webview, [])

    children = [
      {WebView, webview_config}
    ]

    opts = [strategy: :one_for_one, name: Minimal.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
