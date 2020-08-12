idf_component_register(SRCS "main.cpp" "main.hpp"
                            "can_interface.cpp" "can_interface.hpp"
                            "data_storage.hpp"
                            "demo.cpp" "demo.hpp"
                            "status.cpp" "status.hpp"
                            "webserver.cpp" "webserver.hpp"
                            "wifi.cpp" "wifi.hpp"
                       INCLUDE_DIRS "."
                       EMBED_FILES html/index.html html/avionics.js.gz html/favicon.ico)
