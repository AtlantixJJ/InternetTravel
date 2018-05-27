from django.shortcuts import render
from django.http import HttpResponse
import json
import home.api as api


def index(request):
    return render(request, 'index.html')
    # return HttpResponse(index_temp.render())

### Get the nearest node on map
def nearest_node(request):
    form_data = request.GET
    if request.method == 'GET' and 'location' in form_data:
        try:
            lon, lat = map(float, form_data['location'].split(','))
            id, lon, lat = api.get_nearest_node(lon, lat)

            return HttpResponse(json.dumps({
                'succes': True,
                'location': [lon, lat],
                'id': id
            }))
        except Exception as e:
            print(e)
            return HttpResponse('{}')
    return HttpResponse('{}')

### Search for taxi
def query(request):
    form_data = request.GET
    if request.method == 'GET' and 'srcId' in form_data and 'dstId' in form_data:
        try:
            srcId = int(form_data['srcId'])
            dstId = int(form_data['dstId'])
            cars = api.query(srcId, dstId)

            return HttpResponse(json.dumps({
                'succes': True,
                'cars': cars,
            }))
        except Exception as e:
            print(e)
            return HttpResponse('{}')
    return HttpResponse('{}')
