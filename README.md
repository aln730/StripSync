Wi-Fi controlled LED strip system powered by an ESP32.

                +----------------------+
                |   StripSync Server   |  (Go)
                | - WebSocket Hub      |
                | - Effect Engine      |
                +----------------------+
                      ↑       ↑       ↑
                  ws://     ws://   ws://
                  ESP32     ESP32   ESP32
                [Node A]  [Node B] [Node C]
