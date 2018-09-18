defmodule WebView.MixProject do
  use Mix.Project

  def project do
    [
      app: :webview,
      version: "0.0.0",
      elixir: "~> 1.7",
      start_permanent: Mix.env() == :prod,
      compilers: [:elixir_make] ++ Mix.compilers(),
      deps: deps(),
      name: "WebView",
      source_url: "https://github.com/und0ck3d/webview",
      homepage_url: "https://github.com/und0ck3d/webview",
      docs: [
        main: "readme",
        extras: ["README.md"]
      ]
    ]
  end

  def application do
    [
      extra_applications: [:logger]
    ]
  end

  defp deps do
    [
      {:elixir_make, "~> 0.4"},
      {:ex_doc, "~> 0.19", only: :dev, runtime: false},
      {:credo, "~> 0.10.0", only: [:dev, :test], runtime: false}
    ]
  end
end
