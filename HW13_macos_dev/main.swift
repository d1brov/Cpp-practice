import Foundation

let jsonData = """
{
    "candidates": [
        {
          "geometry": {
            "location": {
              "lat": 48.470247,
              "lng": 35.036206
            }
          },
          "name": "Coffee Life"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.469434,
              "lng": 35.038985
            }
          },
          "name": "Have Coffee"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.466561,
              "lng": 35.045186
            }
          },
          "name": "AROMA KAVA"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.463783,
              "lng": 35.042595
            }
          },
          "name": "Mr. Coffee"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.463516,
              "lng": 35.048889
            }
          },
          "name": "DoubleDecker"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.425694,
              "lng": 35.025487
            }
          },
          "name": "Mocco bar"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.451214,
              "lng": 35.057789
            }
          },
          "name": "Confetti"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.460239,
              "lng": 35.069301
            }
          },
          "name": "One dollar bar"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.459414,
              "lng": 35.064731
            }
          },
          "name": "Heisenberg Coffee Lab"
        },
        {
          "geometry": {
            "location": {
              "lat": 48.423311,
              "lng": 35.058121
            }
          },
          "name": "Sopranos"
        }
    ]
}
""".data(using: .utf8)!

struct Location: Decodable {
    let lat: Float
    let lng: Float
    init(lat: Float, lng: Float) {
        self.lat = lat
        self.lng = lng
    }
}

struct Geometry: Decodable {
    let location: Location
}

struct Candidate: Decodable {
    let name: String
    let geometry: Geometry
}

struct Places: Decodable {
    var candidates: [Candidate]
}

class CoolPlacesNearMe {
    var basePoint: Location
    var candidates: [Candidate]
    init(basePoint: Location, candidates: [Candidate]) {
        self.basePoint = basePoint
        self.candidates = candidates
    }
    
    func printPlacesIn(radiusKm: Float) {
        print("Places in radius", radiusKm, "km:")
        for candidate in candidates {
            let dst = distanceFromBP(otherPoint: candidate.geometry.location);
            if (dst <= radiusKm) {
                print(" ", candidate.name, ":", dst, "km")
            }
        }
    }
    
    private func distanceFromBP(otherPoint: Location) -> Float {
        
        let R:(Float) = 6371000 // Earth radius m
        let M_PI_180:(Float) = Float.pi / 180 // C style constant Pi/180
        
        let fi1 = self.basePoint.lat * M_PI_180
        let fi2 = otherPoint.lat * M_PI_180
        let deltaFi = (otherPoint.lat - self.basePoint.lat) * M_PI_180
        let deltaLamda = (otherPoint.lng - self.basePoint.lng) * M_PI_180
        
        let a = pow(sin(deltaFi/2),2) + cos(fi1) * cos(fi2) * pow(sin(deltaLamda/2), 2)
        let c = 2 * atan2(sqrt(a), sqrt(1-a))
        let d = R * c
        
        return d / 1000.0
    }
}

let basePoint = Location(lat: 48.471207, lng: 35.038810)
let places: Places = try! JSONDecoder().decode(Places.self, from: jsonData)

let coolPlacesNearMe = CoolPlacesNearMe(
                            basePoint: basePoint,
                            candidates: places.candidates)

coolPlacesNearMe.printPlacesIn(radiusKm: 0.1)
print()
coolPlacesNearMe.printPlacesIn(radiusKm: 0.2)
print()
coolPlacesNearMe.printPlacesIn(radiusKm: 0.5)
print()
coolPlacesNearMe.printPlacesIn(radiusKm: 5)