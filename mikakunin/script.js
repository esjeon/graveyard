
var video_url = 'mikakunin.mp4'
  , fps = 23.975
  , interval = 1000 / fps
  , hue = {
    kobeni: [250/255,  45/255, 164/255],
    benio : [ 47/255, 176/255, 244/255],
    masiro: [249/255, 119/255,  83/255],
    hakuya: [255/255, 255/255, 255/255],
  }

var video   = $html('video')
  , vmirror = $html('canvas').$ctx()
  , vmask   = new Image()
  , rectfly = $html('canvas').$ctx()

var framebuf = $html('canvas').$ctx()
  , rects = []

function render()
{
  if(self.video.paused || self.video.ended) return

  framebuf.drawImage(video, 0, 0)

  vmirror.globalCompositeOperation = "copy"
  vmirror.drawImage(video, 0, 0)
  vmirror.globalCompositeOperation = "destination-atop"
  vmirror.drawImage(vmask, 0, 0)

  rectfly.clearRect(0, 0, rectfly.width, rectfly.height)
  rects.forEach(function(rect) {
    rect.process(framebuf, rectfly)
  })

  setTimeout(render, interval)
}

function zoomToFit(pageWidth, pageHeight) {
  var rh = window.innerHeight / pageHeight
    , rw = window.innerWidth  / pageWidth
  document.body.style.zoom = Math.floor(Math.min(rw, rh) * 100) + "%"
}

function generateMask() {
  var u = Math.PI / 5
  return $html("canvas")
    .$size(video.vwidth, video.vheight)
    .$ctx("2d", function(ctx) {
      ctx.$frame().$xyeach(function(x, y, i) {
        var s = Math.cos(u * y) * Math.cos(u * x)
        this.$rgba(i, 255, 255, 255, 255 - 255 * ((s > 0)? 1-s: 1-s*s))
      })
      .$put(ctx, 0, 0)
    })
    .toDataURL()
}

function initialize() {
  var css = {
    'width '   : video.vwidth  + 'px',
    'height'   : video.vheight + 'px',
    'position' : 'absolute',
    'top'      : '50%',
    'marginTop': -video.vheight / 2 + 'px',
  }

  vmirror.canvas.$addto(document.body).$style(css).$size(video.vwidth, video.vheight)
  rectfly.canvas.$addto(document.body).$style(css).$size(video.vwidth, video.vheight)

  vmirror.canvas.style.zOrder = 10
  rectfly.canvas.style.zOrder = 11

  //rectfly.globalAlpha = 1
  rectfly.globalCompositeOperation = "source-over"

  framebuf.globalCompositeOperation = "copy"
  framebuf.canvas.$size(video.vwidth, video.vheight)


  document.body.$style({
    'width' : video.vwidth  + 'px',
    'height': video.vheight + 'px',
    'margin': "auto",
    'background': "white",
  })

  window.addEventListener('resize', zoomToFit.bind(null, video.vwidth, video.vheight))
  zoomToFit(video.vwidth, video.vheight)

  vmask.src = generateMask();

  (function() {
    var timer, cnt = 0
    timer = setInterval(function() {
        if(++cnt == 7) clearInterval(timer)
        for(name in hue)
          rects.push(new FlyingRect(hue[name]))
        rects.push(new FlyingRect(null))
      }, 200)
  }) ()

  video.play()
}

window.onload = function() {
  video.$attr({
      'src': video_url,
      'loop': true,
    })
    .$ev('loadeddata', initialize)
    .$ev('play', function() {
      setTimeout(render, 0)
    })
    .load()
}

/*
 * FlyingRect
 *
 */

function rgbAverageHued(imd, rw, gw, bw, rh, gh, bh) {
  for(var i = 0; i < imd.data.length; i += 4) {
    var l = rw * imd.data[i] + gw * imd.data[i+1] + bw * imd.data[i+2]
    l = l * 0.6 + 255*0.4
    imd.data[i  ] = l * rh
    imd.data[i+1] = l * gh
    imd.data[i+2] = l * bh
  }
}

function FlyingRect(hue)
{
  var buffer = $html('canvas').$ctx()

  var w, h, y
    , spd, skip
    , begin = 0

  this.initialize = function() {
    w = Math.round(Math.rand(0.40, 0.50) * video.vwidth)
    h = Math.round(Math.rand(0.10, 0.22) * video.vheight)
    y = Math.round(Math.rand(0.00, 1.00) * (video.vheight - h))
    spd = (video.vwidth) / Math.rand(2, 5) / 1000
    skip = Math.round(Math.rand(0,fps))

    buffer.canvas.$size(w, h)
  }

  this.process = function(ictx, octx) {
    var x

    if(--skip > 0)
      return
    else if(skip == 0) {
      begin = new Date().getTime()
      x = -w + 2
    } else {
      x = spd * (new Date().getTime() - begin) - w
      if(x > video.vwidth) {
        this.initialize()
        return
      }
    }

    var left = Math.max(0, x)
      , width = Math.min(x + w, video.vwidth) - left

    buffer.drawImage(ictx.canvas, left, y, width, h, 0, 0, width, h)

    if(hue) {
      var imd = buffer.getImageData(0, 0, width, h)
      rgbAverageHued(imd, 0.21, 0.70, 0.08, hue[0], hue[1], hue[2])
      buffer.putImageData(imd, 0, 0)
    }

    octx.drawImage(buffer.canvas, 0, 0, width, h, left, y, width, h)
  }

  this.initialize()
}

