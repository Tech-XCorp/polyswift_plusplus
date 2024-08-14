MathJax.Hub.Config({
  jax: ["input/TeX","input/MathML","output/SVG", "output/PreviewHTML"],
  extensions: ["tex2jax.js","mml2jax.js","MathMenu.js","MathZoom.js", "fast-preview.js", "AssistiveMML.js"],
  TeX: {
    extensions: ["AMSmath.js","AMSsymbols.js","noErrors.js","noUndefined.js"]
  }
});
MathJax.Ajax.loadComplete("[MathJax]/config/mathjaxconf.js")