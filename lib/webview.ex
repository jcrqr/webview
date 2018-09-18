defmodule WebView do
  @moduledoc """
  WebView API
  """

  use GenServer

  alias WebView.Native
  alias WebView.Settings

  @doc """
  Returns a specification to start `WebView` under a supervisor.

  See `Supervisor`.
  """
  def child_spec(arg) do
    spec = %{
      id: __MODULE__,
      start: {__MODULE__, :start, [arg]}
    }

    Supervisor.child_spec(spec, [])
  end

  @doc """
  Starts the `WebView`. For a list of available options check the
  `WebView.Settings` documentation.
  """
  def start(opts \\ []) do
    GenServer.start_link(__MODULE__, opts, name: __MODULE__)
  end

  @doc """
  Sets the `WebView` title.
  """
  def set_title(title, timeout \\ 1000) do
    GenServer.call(__MODULE__, {:set_title, title}, timeout)
  end

  @doc """
  Sets the `WebView` fullscreen on/off.
  """
  def set_fullscreen(fullscreen \\ true, timeout \\ 1000) do
    GenServer.call(__MODULE__, {:set_fullscreen, fullscreen}, timeout)
  end

  @doc """
  Evaluates JavaScript code.
  """
  def eval(js, timeout \\ 1000) do
    GenServer.call(__MODULE__, {:eval, js}, timeout)
  end

  @doc """
  Invoked when `WebView` is started. Initializes the actual `webview` and
  then starts the loop.
  """
  def init(opts \\ []) do
    settings = struct!(Settings, opts)

    send(self(), :create)

    {:ok, %{settings: settings, created?: false, running?: false}}
  end

  @doc false
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

  @doc false
  def handle_info(:loop, state) do
    running? =
      case Native.loop(0) do
        :ok ->
          send(self(), :loop)
          true

        :stop ->
          false
      end

    {:noreply, %{state | running?: running?}}
  end

  def handle_info(:create, %{created?: true} = state), do: {:noreply, state}

  def handle_info(:create, %{settings: %Settings{size: {w, h}} = s} = state) do
    :ok = Native.create(s.title, s.url, w, h, bool_to_int(s.resizable), bool_to_int(s.debug))

    Process.send_after(self(), :loop, 10)

    {:noreply, state}
  end

  defp bool_to_int(true), do: 1
  defp bool_to_int(false), do: 0
end
