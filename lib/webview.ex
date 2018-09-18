defmodule WebView do
  use GenServer

  alias WebView.Native

  def start(opts \\ []) do
    GenServer.start_link(__MODULE__, opts, name: __MODULE__)
  end

  def set_title(title, timeout \\ 5000) do
    GenServer.call(__MODULE__, {:set_title, title}, timeout)
  end

  def set_fullscreen(fullscreen \\ true, timeout \\ 5000) do
    GenServer.call(__MODULE__, {:set_fullscreen, fullscreen}, timeout)
  end

  def eval(js, timeout \\ 5000) do
    GenServer.call(__MODULE__, {:eval, js}, timeout)
  end

  def init(opts \\ []) do
    title = Keyword.get(opts, :title, "WebView")
    url = Keyword.get(opts, :url, "https://elixir-lang.org")
    width = Keyword.get(opts, :width, 800)
    height = Keyword.get(opts, :height, 600)
    resizable = if Keyword.get(opts, :resizable, true), do: 1, else: 0
    debug = if Keyword.get(opts, :debug, true), do: 1, else: 0

    :ok = Native.create(title, url, width, height, resizable, debug)

    Process.send_after(self(), :loop, 1000)

    {:ok, nil}
  end

  def handle_call({:set_title, title}, _, state) do
    Native.set_title(title)

    {:reply, :ok, state}
  end

  def handle_call({:set_fullscreen, fullscreen}, _, state) do
    fullscreen = if fullscreen, do: 1, else: 0

    Native.set_fullscreen(fullscreen)

    {:reply, :ok, state}
  end

  def handle_call({:eval, js}, _, state) do
    Native.eval(js)

    {:reply, :ok, state}
  end

  def handle_call({:inject_css, css}, _, state) do
    Native.inject_css(css)

    {:reply, :ok, state}
  end

  def handle_info(:loop, state) do
    Native.loop(0)

    send(self(), :loop)

    {:noreply, state}
  end
end
