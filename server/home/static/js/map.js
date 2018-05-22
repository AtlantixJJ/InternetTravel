'use strict';
var infoWindow,
  map,
  navi,
  pathSimplifierIns,
  zooms = [9, 18],
  center = { lng: 116.442263, lat: 39.835354 },
  src = null,
  dst = null,
  cars = [],
  srcMark = null,
  dstMark = null,
  carMarks = [],
  otherMarks = [],
  currentVehicleMark = null,
  passIcon = new AMap.Icon({
    size: new AMap.Size(16, 24),
    image: 'static/img/mark_r.png',
    imageSize: new AMap.Size(16, 24),
  }),
  carIcon = new AMap.Icon({
    size: new AMap.Size(16, 246),
    image: 'static/img/mark_b.png',
    imageSize: new AMap.Size(16, 24),
  }),
  carActivedIcon = new AMap.Icon({
    size: new AMap.Size(16, 24),
    image: 'static/img/mark_b.png',
    imageSize: new AMap.Size(16, 24),
  });
function isSameNode(e, n) {
  return e.location[0] === n.location[0] && e.location[1] === n.location[1];
}
function markIcon(e) {
  return new AMap.Icon({
    size: new AMap.Size(24, 28),
    image: 'static/img/marks.png',
    imageSize: new AMap.Size(480, 30),
    imageOffset: new AMap.Pixel(24 * -e, 0),
  });
}
function newMark(e, n, t) {
  return new AMap.Marker({
    map: map,
    position: e,
    topWhenClick: !0,
    clickable: !0,
    offset: new AMap.Pixel(-12, -27),
    icon: 'number' == typeof n ? markIcon(n) : n,
    label: void 0 !== t ? { content: '<div class="mark-label">' + t + '</div>' } : null,
  });
}
function showVehicles(e) {
  clearMarks(),
    e.forEach(function(n, t) {
      for (var a = 0; a < t; a++)
        isSameNode(e[a], n) &&
          ((e[t].location[0] += (Math.random() - 0.5) / 5e3),
          (e[t].location[1] += (Math.random() - 0.5) / 5e3));
    }),
    e.forEach(function(e, n) {
      carMarks[n] ||
        ((carMarks[n] = newMark(e.location, carIcon)),
        AMap.event.addListener(carMarks[n], 'click', function(e) {
          currentVehicleMark && currentVehicleMark.setIcon(carIcon),
            (currentVehicleMark = carMarks[n]),
            carMarks[n].setIcon(carActivedIcon),
            showVehiclePath(n);
        })),
        carMarks[n].setPosition(e.location),
        carMarks[n].show();
    });
}
function showPassengers(e) {
  otherMarks.forEach(function(e) {
    return e.hide();
  }),
    e.forEach(function(e, n) {
      otherMarks[n] || ((otherMarks[n] = newMark(e, passIcon)), otherMarks[n].setIcon(passIcon)),
        otherMarks[n].setPosition(e),
        otherMarks[n].show();
    });
}
function showVehiclePath(e) {
  if (pathSimplifierIns) {
    var n = cars[e];
    showPassengers(n.passengers), pathSimplifierIns.setData([{ path: n.path }]);
  }
}
function clearMarks() {
  var e = arguments.length > 0 && void 0 !== arguments[0] && arguments[0];
  currentVehicleMark && (currentVehicleMark.setIcon(carIcon), (currentVehicleMark = null)),
    pathSimplifierIns && pathSimplifierIns.setData(null),
    otherMarks.forEach(function(e) {
      return e.hide();
    }),
    carMarks.forEach(function(e) {
      return e.hide();
    }),
    e &&
      ((src = dst = null),
      srcMark && srcMark.hide(),
      dstMark && dstMark.hide(),
      $('#btn-action').text('选择起点'),
      $('#btn-action').prop('disabled', !1));
}
function onClickActionButton() {
  src && dst
    ? getSolution(src.id, dst.id, function(e) {
        (cars = e.cars).length
          ? (showVehicles(cars), map.setFitView(), $('#btn-action').prop('disabled', !0))
          : alert('找不到出租车！');
      })
    : $('#btn-action').prop('disabled', !0);
}
function onClickMap(e) {
  $('#btn-action').prop('disabled') &&
    (src
      ? dst ||
        getNearestNode(e, function(e) {
          (dst = e),
            dstMark || (dstMark = newMark(dst.location, 1, '终')),
            dstMark.setPosition(dst.location),
            dstMark.show(),
            $('#btn-action').prop('disabled', !1),
            $('#btn-action').text('拼车');
        })
      : getNearestNode(e, function(e) {
          (src = e),
            srcMark || (srcMark = newMark(src.location, 7, '起')),
            srcMark.setPosition(src.location),
            srcMark.show(),
            $('#btn-action').prop('disabled', !1),
            $('#btn-action').text('选择终点');
        }));
}
function initMap() {
  (map = new AMap.Map('container', {
    center: new AMap.LngLat(center.lng, center.lat),
    resizeEnable: !0,
    zooms: zooms,
  })).on('click', function(e) {
    onClickMap([e.lnglat.getLng(), e.lnglat.getLat()]);
  }),
    AMap.event.addListener(map, 'zoomend', function() {
      navi && navi.setSpeed(getVehicleSpeed(map.getZoom()));
    }),
    document.getElementById('btn-action').addEventListener('click', onClickActionButton, !1),
    document.getElementById('btn-clear').addEventListener(
      'click',
      function() {
        return clearMarks(!0);
      },
      !1
    );
}
function initPathSimplifier(e) {
  pathSimplifierIns = new e({
    zIndex: 100,
    map: map,
    getPath: function(e, n) {
      return e.path;
    },
    getHoverTitle: function(e, n, t) {
      return null;
    },
    clickToSelectPath: !1,
    autoSetFitView: !1,
    renderOptions: {
      pathLineStyle: { strokeStyle: '#99aa22', lineWidth: 6, dirArrowStyle: !0 },
      pathLineHoverStyle: null,
    },
  });
}
function getNearestNode(e, n) {
  $.get('nearestnode', { location: e.join(',') }, n, 'json');
}
function getSolution(e, n, t) {
  $.get('query', { srcId: e, dstId: n }, t, 'json');
}
$(document).ready(function() {
  initMap(),
    AMapUI.load(['ui/misc/PathSimplifier'], function(e) {
      initPathSimplifier(e);
    });
});