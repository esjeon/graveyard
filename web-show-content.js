/* 
 * This is a simple bookmarklet which extracts the content of the page and
 * display it in plain HTML style.
 *
 * use http://marijnhaverbeke.nl/uglifyjs to simplify the code
 */

(function() {
  var q = document.querySelector.bind(document)
    , qa= document.querySelectorAll.bind(document)
    , c = (q('article') || q('div#content')).innerHTML
  document.clear();
  document.write(c);
  document.close();

  function foreach(f,arr) {
    Array.prototype.forEach.call(arr, f);
  }
    
  foreach(function(el){
    el.parentNode.removeChild(el);
  }, qa('link[type="text/css"]'));
  
  with(document.body.style){
    fontSize = "15px";
    maxWidth = "95%";
    width    = "40em";
    margin   = "auto";
  }

  foreach(function(el) {
    el.style.background = "#ddd";
  }, qa('pre'));
}) ()
