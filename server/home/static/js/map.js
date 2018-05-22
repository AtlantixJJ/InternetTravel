var infoWindow,
  map,
  navi,
  zooms = [9, 18],
  center = { lng: 116.442263, lat: 39.835354 },
  pathSimplifierIns;

var src = null,
  dst = null,
  cars = [];

var srcMark = null,
  dstMark = null,
  carMarks = [],
  otherMarks = [],
  currentVehicleMark = null;

var passIcon = new AMap.Icon({
  size: new AMap.Size(16, 24),
  image: "static/img/mark_r.png",
  imageSize: new AMap.Size(16, 24)
});
var carIcon = new AMap.Icon({
  size: new AMap.Size(16, 246),
  image: "static/img/mark_b.png",
  imageSize: new AMap.Size(16, 24)
});
var carActivedIcon = new AMap.Icon({
  size: new AMap.Size(16, 24),
  image: "static/img/mark_b.png",
  imageSize: new AMap.Size(16, 24)
});

function isSameNode(a, b) {
  return a.location[0] === b.location[0] && a.location[1] === b.location[1];
}

function markIcon(color) {
  return new AMap.Icon({
    size: new AMap.Size(24, 28),
    image: "static/img/marks.png",
    imageSize: new AMap.Size(480, 30),
    imageOffset: new AMap.Pixel(-color * 24, 0)
  });
}

function newMark(position, icon, label) {
  return new AMap.Marker({
    map: map,
    position: position,
    topWhenClick: true,
    clickable: true,
    offset: new AMap.Pixel(-12, -27),
    icon: typeof icon == "number" ? markIcon(icon) : icon,
    label:
      label !== undefined
        ? {
            content: `<div class="mark-label">${label}</div>`
          }
        : null
  });
}

function showVehicles(cars) {
  clearMarks();

  cars.forEach((car, i) => {
    for (let j = 0; j < i; j++)
      if (isSameNode(cars[j], car)) {
        // 车辆有时候会在同一个点上，需要把他们略微分开
        cars[i].location[0] += (Math.random() - 0.5) / 5000;
        cars[i].location[1] += (Math.random() - 0.5) / 5000;
      }
  });
  cars.forEach((car, i) => {
    if (!carMarks[i]) {
      carMarks[i] = newMark(car.location, carIcon);

      AMap.event.addListener(carMarks[i], "click", event => {
        if (currentVehicleMark) currentVehicleMark.setIcon(carIcon);
        currentVehicleMark = carMarks[i];
        carMarks[i].setIcon(carActivedIcon);
        showVehiclePath(i);
      });
    }
    carMarks[i].setPosition(car.location);
    carMarks[i].show();
  });
}

// 车上的其他乘客
function showPassengers(pass) {
  otherMarks.forEach(mark => mark.hide());
  pass.forEach((position, i) => {
    if (!otherMarks[i]) {
      otherMarks[i] = newMark(position, passIcon);
      otherMarks[i].setIcon(passIcon);
    }
    otherMarks[i].setPosition(position);
    otherMarks[i].show();
  });
}

function showVehiclePath(index) {
  if (!pathSimplifierIns) return;

  let car = cars[index];
  showPassengers(car.passengers);

  pathSimplifierIns.setData([
    {
      path: car.path
    }
  ]);
}

function clearMarks(clearSrcDst = false) {
  if (currentVehicleMark) {
    currentVehicleMark.setIcon(carIcon);
    currentVehicleMark = null;
  }
  pathSimplifierIns && pathSimplifierIns.setData(null);
  otherMarks.forEach(mark => mark.hide());
  carMarks.forEach(mark => mark.hide());
  if (clearSrcDst) {
    src = dst = null;
    srcMark && srcMark.hide();
    dstMark && dstMark.hide();
    $("#btn-action").text("选择起点");
    $("#btn-action").prop("disabled", false);
  }
}

function onClickActionButton() {
  if (!src) {
    $("#btn-action").prop("disabled", true);
  } else if (!dst) {
    $("#btn-action").prop("disabled", true);
  } else {
    getSolution(src.id, dst.id, data => {
      cars = data.cars;
      if (!cars.length) {
        alert("找不到出租车！");
        return;
      }
      showVehicles(cars);
      map.setFitView();
      $("#btn-action").prop("disabled", true);
    });
  }
}

function onClickMap(position) {
  if (!$("#btn-action").prop("disabled")) return;
  if (!src) {
    getNearestNode(position, data => {
      src = data;
      if (!srcMark) srcMark = newMark(src.location, 7, "起");
      srcMark.setPosition(src.location);
      srcMark.show();
      $("#btn-action").prop("disabled", false);
      $("#btn-action").text("选择终点");
    });
  } else if (!dst) {
    getNearestNode(position, data => {
      dst = data;
      if (!dstMark) dstMark = newMark(dst.location, 1, "终");
      dstMark.setPosition(dst.location);
      dstMark.show();
      $("#btn-action").prop("disabled", false);
      $("#btn-action").text("拼车");
    });
  }
}

function initMap() {
  map = new AMap.Map("container", {
    center: new AMap.LngLat(center.lng, center.lat),
    resizeEnable: true,
    zooms: zooms
  });

  map.on("click", function(e) {
    onClickMap([e.lnglat.getLng(), e.lnglat.getLat()]);
  });

  AMap.event.addListener(map, "zoomend", function() {
    if (navi) navi.setSpeed(getVehicleSpeed(map.getZoom()));
  });
  document
    .getElementById("btn-action")
    .addEventListener("click", onClickActionButton, false);
  document
    .getElementById("btn-clear")
    .addEventListener("click", () => clearMarks(true), false);
}

function initPathSimplifier(PathSimplifier) {
  pathSimplifierIns = new PathSimplifier({
    zIndex: 100,
    map: map,
    getPath: function(pathData, pathIndex) {
      return pathData.path;
    },
    getHoverTitle: function(pathData, pathIndex, pointIndex) {
      return null;
    },
    clickToSelectPath: false,
    autoSetFitView: false,
    renderOptions: {
      pathLineStyle: {
        strokeStyle: "#99aa22",
        lineWidth: 6,
        dirArrowStyle: true
      },
      pathLineHoverStyle: null
    }
  });
}

function getNearestNode(position, callback) {
  $.get(
    "nearestnode",
    {
      location: position.join(",")
    },
    callback,
    "json"
  );
}

function getSolution(srcId, dstId, callback) {
  $.get(
    "query",
    {
      srcId: srcId,
      dstId: dstId
    },
    callback,
    "json"
  );
}

$(document).ready(() => {
  initMap();

  AMapUI.load(["ui/misc/PathSimplifier"], function(PathSimplifier) {
    initPathSimplifier(PathSimplifier);
  });
});
