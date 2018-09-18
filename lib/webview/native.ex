defmodule WebView.Native do
  @moduledoc """
  Native Implemented Functions
  """

  @on_load :load_nif

  @doc false
  def load_nif do
    :webview |> :code.priv_dir() |> Path.join("webview") |> :erlang.load_nif(0)
  end

  @doc """
  Creates a new `WebView`. Returns `:ok` in case of success
  and `{:error, :already_created}` if the `WebView` has already been created.
  """
  @spec create(String.t(), String.t(), Integer.t(), Integer.t(), boolean(), boolean()) ::
          :ok | {:error, :already_created}
  def create(title, url, width, height, resizable, debug)
  def create(_, _, _, _, _, _), do: not_loaded()

  @doc """
  Calls the loop function, either blocking or non-blocking.
  """
  @spec loop(boolean()) :: :ok
  def loop(blocking)
  def loop(_), do: not_loaded()

  @doc """
  Sets the `WebView` title.
  """
  @spec set_title(String.t()) :: :ok
  def set_title(title)
  def set_title(_), do: not_loaded()

  @doc """
  Toggles `WebView` fullscreen.
  """
  @spec set_fullscreen(boolean()) :: :ok
  def set_fullscreen(fullscreen)
  def set_fullscreen(_), do: not_loaded()

  @doc """
  Evaluates JavaScript code in the `WebView`.
  """
  @spec eval(String.t()) :: :ok
  def eval(js)
  def eval(_), do: not_loaded()

  @doc """
  Injects CSS in the `WebView`.
  """
  @spec inject_css(String.t()) :: :ok
  def inject_css(css)
  def inject_css(_), do: not_loaded()

  defp not_loaded, do: :erlang.nif_error(:not_loaded)
end
