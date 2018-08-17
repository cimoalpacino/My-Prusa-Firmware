/**
 * Spanish
 *
 * LCD Menu Messages
 * Please note these are limited to 17 characters!
 *
 */

#define WELCOME_MSG                         CUSTOM_MENDEL_NAME " prep."
#define MSG_SD_INSERTED                     "Tarjeta insertada"
#define MSG_SD_REMOVED                      "Tarjeta retirada"
#define MSG_MAIN                            "Menu principal"
#define MSG_DISABLE_STEPPERS                "Apagar motores"
#define MSG_AUTO_HOME                       "Llevar al origen"
#define MSG_COOLDOWN                        "Enfriar"
#define MSG_MOVE_AXIS                       "Mover ejes"
#define MSG_MOVE_X                          "Mover X"
#define MSG_MOVE_Y                          "Mover Y"
#define MSG_MOVE_Z                          "Mover Z"
#define MSG_MOVE_E                          "Extruir"
#define MSG_SPEED                           "Velocidad"
#define MSG_NOZZLE                          "Nozzle"
#define MSG_BED                             "Heatbed"
#define MSG_FAN_SPEED                       "Velocidad Vent."
#define MSG_FLOW                            "Flujo"
#define MSG_TEMPERATURE                     "Temperatura"
#define MSG_WATCH                           "Monitorizar"
#define MSG_TUNE                            "Ajustar"
#define MSG_PAUSE_PRINT                     "Pausar impresion"
#define MSG_RESUME_PRINT                    "Reanudar impres."
#define MSG_STOP_PRINT                      "Detener impresion"
#define MSG_CARD_MENU                       "Menu tarjeta SD"
#define MSG_NO_CARD                         "No hay tarjeta SD"
#define MSG_DWELL                           "En espera"
#define MSG_USERWAIT                        "Esperando ordenes"
#define MSG_RESUMING                        "Resumiendo impresion"
#define MSG_PRINT_ABORTED                   "Impresion cancelada" 
#define MSG_NO_MOVE                         "Sin movimiento"
#define MSG_KILLED                          "PARADA DE EMERGENCIA"
#define MSG_STOPPED                         "PARADA"
#define MSG_FILAMENTCHANGE                  "Cambiar filamento"
#define MSG_BABYSTEP_Z                      "Micropaso Eje Z"
#define MSG_ADJUSTZ			    "Ajustar Eje Z"			
#define MSG_PICK_Z			    "Esc. Modelo Adecuado"

#define MSG_SETTINGS                        "Configuracion"
#define MSG_PREHEAT                         "Precalentar"
#define MSG_UNLOAD_FILAMENT                 "Soltar filamento"
#define MSG_LOAD_FILAMENT		    "Introducir filam."
#define MSG_ERROR							"ERROR:"
#define MSG_PREHEAT_NOZZLE                  "Precalentar extrusor"
#define MSG_SUPPORT			    "Soporte"
#define MSG_CORRECTLY			    "Cambiado correct.?"
#define MSG_YES								"Si"
#define MSG_NO								"No"
#define MSG_NOT_LOADED 			    "Fil. no introducido"
#define MSG_NOT_COLOR 			    "Color no homogeneo"
#define MSG_LOADING_FILAMENT		    "Introduciendo filam."
#define MSG_PLEASE_WAIT			    "Por Favor Esperar"
#define MSG_LOADING_COLOR                   "Cambiando color" 
#define MSG_CHANGE_SUCCESS		    "Cambio correcto"
#define MSG_PRESS			    "Haz clic"
#define MSG_INSERT_FILAMENT		    "Introducir filamento"
#define MSG_CHANGING_FILAMENT		    "Cambiando filamento"
#define MSG_SILENT_MODE_ON					"Modo   [silencio]"
#define MSG_SILENT_MODE_OFF		    "Modo [rend.pleno]" 
#define MSG_AUTO_MODE_ON			"Modo       [auto]"
#define MSG_REBOOT			    "Reiniciar impresora"
#define MSG_TAKE_EFFECT			    " para aplicar cambios" 
#define MSG_HEATING                         "Calentando..."
#define MSG_HEATING_COMPLETE                "Calentamiento final."
#define MSG_BED_HEATING                     "Calentando Base"
#define MSG_BED_DONE                        "Base preparada"
#define MSG_LANGUAGE_NAME					"Espanol"
#define MSG_LANGUAGE_SELECT		    "Cambiae el idioma"
#define MSG_PRUSA3D							"prusa3d.com"
#define MSG_PRUSA3D_FORUM					"forum.prusa3d.com"
#define MSG_PRUSA3D_HOWTO					"howto.prusa3d.com"


// Do not translate those!

#define MSG_Enqueing                        "enqueing \""
#define MSG_POWERUP                         "PowerUp"
#define MSG_CONFIGURATION_VER               " Last Updated: "
#define MSG_FREE_MEMORY                     " Free Memory: "
#define MSG_PLANNER_BUFFER_BYTES            "  PlannerBufferBytes: "
#define MSG_OK                              "ok"
#define MSG_ERR_CHECKSUM_MISMATCH           "checksum mismatch, Last Line: "
#define MSG_ERR_NO_CHECKSUM                 "No Checksum with line number, Last Line: "
#define MSG_BEGIN_FILE_LIST                 "Begin file list"
#define MSG_END_FILE_LIST                   "End file list"
#define MSG_M104_INVALID_EXTRUDER           "M104 Invalid extruder "
#define MSG_M105_INVALID_EXTRUDER           "M105 Invalid extruder "
#define MSG_M200_INVALID_EXTRUDER           "M200 Invalid extruder "
#define MSG_M218_INVALID_EXTRUDER           "M218 Invalid extruder "
#define MSG_M221_INVALID_EXTRUDER           "M221 Invalid extruder "
#define MSG_ERR_NO_THERMISTORS              "No thermistors - no temperature"
#define MSG_M109_INVALID_EXTRUDER           "M109 Invalid extruder "
#define MSG_M115_REPORT                     "FIRMWARE_NAME:Marlin V1.0.2; Sprinter/grbl mashup for gen6 FIRMWARE_URL:" FIRMWARE_URL " PROTOCOL_VERSION:" PROTOCOL_VERSION " MACHINE_TYPE:" CUSTOM_MENDEL_NAME " EXTRUDER_COUNT:" STRINGIFY(EXTRUDERS) " UUID:" MACHINE_UUID "\n"
#define MSG_ERR_KILLED                      "Printer halted. kill() called!"
#define MSG_ERR_STOPPED                     "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define MSG_RESEND                          "Resend: "
#define MSG_M119_REPORT                     "Reporting endstop status"
#define MSG_ENDSTOP_HIT                     "TRIGGERED"
#define MSG_ENDSTOP_OPEN                    "open"
#define MSG_SD_CANT_OPEN_SUBDIR             "Cannot open subdir"
#define MSG_SD_INIT_FAIL                    "SD init fail"
#define MSG_SD_VOL_INIT_FAIL                "volume.init failed"
#define MSG_SD_OPENROOT_FAIL                "openRoot failed"
#define MSG_SD_CARD_OK                      "SD card ok"
#define MSG_SD_WORKDIR_FAIL                 "workDir open failed"
#define MSG_SD_OPEN_FILE_FAIL               "open failed, File: "
#define MSG_SD_FILE_OPENED                  "File opened: "
#define MSG_SD_FILE_SELECTED                "File selected"
#define MSG_SD_WRITE_TO_FILE                "Writing to file: "
#define MSG_SD_PRINTING_BYTE                "SD printing byte "
#define MSG_SD_NOT_PRINTING                 "Not SD printing"
#define MSG_SD_ERR_WRITE_TO_FILE            "error writing to file"
#define MSG_SD_CANT_ENTER_SUBDIR            "Cannot enter subdir: "
#define MSG_STEPPER_TOO_HIGH                "Steprate too high: "
#define MSG_ENDSTOPS_HIT                    "endstops hit: "
#define MSG_ERR_COLD_EXTRUDE_STOP           " cold extrusion prevented"
#define MSG_BABYSTEPPING_X                  "Babystepping X"
#define MSG_BABYSTEPPING_Y                  "Babystepping Y"
#define MSG_BABYSTEPPING_Z                  "Ajustar Z"
#define MSG_SERIAL_ERROR_MENU_STRUCTURE     "Error in menu structure"
#define MSG_SET_HOME_OFFSETS                "Set home offsets"
#define MSG_SET_ORIGIN                      "Set origin"
#define MSG_SWITCH_PS_ON                    "Switch power on"
#define MSG_SWITCH_PS_OFF                   "Switch power off"
#define MSG_NOZZLE1                         "Nozzle2"
#define MSG_NOZZLE2                         "Nozzle3"
#define MSG_FLOW0                           "Flow 0"
#define MSG_FLOW1                           "Flow 1"
#define MSG_FLOW2                           "Flow 2"
#define MSG_CONTROL                         "Control"
#define MSG_MIN                             " \002 Min"
#define MSG_MAX                             " \002 Max"
#define MSG_FACTOR                          " \002 Fact"
#define MSG_MOTION                          "Motion"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED		        "E in mm3"
#define MSG_STORE_EPROM                     "Store memory"
#define MSG_LOAD_EPROM                      "Load memory"
#define MSG_RESTORE_FAILSAFE                "Restore failsafe"
#define MSG_REFRESH                         "\xF8" "Refresh"
#define MSG_INIT_SDCARD                     "Init. SD card"
#define MSG_CNG_SDCARD                      "Change SD card"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_RECTRACT                        "Rectract"

#define MSG_HOMEYZ                          "Calibrar Z"
#define MSG_HOMEYZ_PROGRESS                 "Calibrando Z"
#define MSG_HOMEYZ_DONE                     "Calibracion OK"

#define MSG_SELFTEST_ERROR                  "Autotest error!"
#define MSG_SELFTEST_PLEASECHECK            "Controla :"   
#define MSG_SELFTEST_NOTCONNECTED           "No hay conexion  "
#define MSG_SELFTEST_HEATERTHERMISTOR       "Hotend"
#define MSG_SELFTEST_BEDHEATER              "Heatbed"
#define MSG_SELFTEST_WIRINGERROR            "Error de conexion"
#define MSG_SELFTEST_ENDSTOPS               "Topes finales"
#define MSG_SELFTEST_MOTOR                  "Motor"
#define MSG_SELFTEST_ENDSTOP                "Tope final"
#define MSG_SELFTEST_ENDSTOP_NOTHIT         "Tope no alcanzado"
#define MSG_SELFTEST_OK                     "Self test OK"

#define MSG_SELFTEST_FAN					"Test ventiladores"
#define MSG_SELFTEST_COOLING_FAN			"Vent. frontal?"
#define MSG_SELFTEST_EXTRUDER_FAN			"Vent. izquierdo?"
#define MSG_SELFTEST_FAN_YES				"Ventilador gira"
#define MSG_SELFTEST_FAN_NO					"Ventilador no gira"

#define MSG_STATS_TOTALFILAMENT             "Filamento total:"
#define MSG_STATS_TOTALPRINTTIME            "Tiempo total :"
#define MSG_STATS_FILAMENTUSED              "Filamento usado:  "
#define MSG_STATS_PRINTTIME                 "Tiempo de imp.:"

#define MSG_SELFTEST_START                  "Inicio Selftest"
#define MSG_SELFTEST_CHECK_ENDSTOPS         "Control topes"
#define MSG_SELFTEST_CHECK_HOTEND           "Control hotend " 
#define MSG_SELFTEST_CHECK_X                "Control tope X"
#define MSG_SELFTEST_CHECK_Y                "Control tope Y"
#define MSG_SELFTEST_CHECK_Z                "Control tope Z"
#define MSG_SELFTEST_CHECK_BED              "Control heatbed"
#define MSG_SELFTEST_CHECK_ALLCORRECT       "Todo bien"
#define MSG_SELFTEST                        "Selftest"
#define MSG_SELFTEST_FAILED                 "Fallo Selftest"

#define MSG_STATISTICS                      "Estadisticas  "
#define MSG_USB_PRINTING                    "Impresion con USB "

#define MSG_SHOW_END_STOPS                  "Ensena tope final"
#define MSG_CALIBRATE_BED                   "Calibra XYZ"
#define MSG_CALIBRATE_BED_RESET             "Reset XYZ calibr."
#define MSG_MOVE_CARRIAGE_TO_THE_TOP        "Calibrando XYZ. Gira el boton para subir el extrusor hasta tocar los topes superiores. Despues haz clic."
#define MSG_MOVE_CARRIAGE_TO_THE_TOP_Z       "Calibrando Z. Gira el boton para subir el extrusor hasta tocar los topes superiores. Despues haz clic."

#define MSG_CONFIRM_NOZZLE_CLEAN            "Limpia nozzle para calibracion. Click cuando acabes."
#define MSG_CONFIRM_CARRIAGE_AT_THE_TOP     "Carros Z izq./der. estan arriba maximo?"
#define MSG_FIND_BED_OFFSET_AND_SKEW_LINE1  "Buscando punto de calibracion heatbed"
#define MSG_FIND_BED_OFFSET_AND_SKEW_LINE2  " de 4"
#define MSG_IMPROVE_BED_OFFSET_AND_SKEW_LINE1   "Mejorando punto de calibracion heatbed"
#define MSG_IMPROVE_BED_OFFSET_AND_SKEW_LINE2   " de 9"
#define MSG_MEASURE_BED_REFERENCE_HEIGHT_LINE1	"Midiendo altura del punto de calibracion"
#define MSG_MEASURE_BED_REFERENCE_HEIGHT_LINE2	" de 9"

#define MSG_FIND_BED_OFFSET_AND_SKEW_ITERATION	"Reiteracion "
#define MSG_BED_SKEW_OFFSET_DETECTION_POINT_NOT_FOUND           "Calibracion XYZ fallada. Puntos de calibracion en heatbed no encontrados."
#define MSG_BED_SKEW_OFFSET_DETECTION_FITTING_FAILED            "Calibracion XYZ fallada. Consulta el manual por favor."
#define MSG_BED_SKEW_OFFSET_DETECTION_PERFECT               "Calibracion XYZ ok. Ejes X/Y perpendiculares. Enhorabuena!"
#define MSG_BED_SKEW_OFFSET_DETECTION_SKEW_MILD             "Calibracion XYZ correcta. Los ejes X / Y estan ligeramente inclinados. Buen trabajo!"
#define MSG_BED_SKEW_OFFSET_DETECTION_SKEW_EXTREME          "Calibracion XYZ correcta. La inclinacion se corregira automaticamente."
#define MSG_BED_SKEW_OFFSET_DETECTION_FAILED_FRONT_LEFT_FAR     "Calibracion XYZ fallada. Punto frontal izquierdo no alcanzable."
#define MSG_BED_SKEW_OFFSET_DETECTION_FAILED_FRONT_RIGHT_FAR        "Calibracion XYZ fallad. Punto frontal derecho no alcanzable."
#define MSG_BED_SKEW_OFFSET_DETECTION_FAILED_FRONT_BOTH_FAR     "Calibracion XYZ fallada. Puntos frontales no alcanzables."
#define MSG_BED_SKEW_OFFSET_DETECTION_WARNING_FRONT_LEFT_FAR        "Calibrazion XYZ comprometida. Punto frontal izquierdo no alcanzable."
#define MSG_BED_SKEW_OFFSET_DETECTION_WARNING_FRONT_RIGHT_FAR       "Calibrazion XYZ comprometida. Punto frontal derecho no alcanzable."
#define MSG_BED_SKEW_OFFSET_DETECTION_WARNING_FRONT_BOTH_FAR        "Calibrazion XYZ comprometida. Puntos frontales no alcanzables."
#define MSG_BED_LEVELING_FAILED_POINT_LOW           				"Nivelacion fallada. Sensor no funciona. Escombros en nozzle? Esperando reset."
#define MSG_BED_LEVELING_FAILED_POINT_HIGH          				"Nivelacion fallada. Sensor funciona demasiado temprano. Esperando reset."
#define MSG_BED_LEVELING_FAILED_PROBE_DISCONNECTED      			"Nivelacion fallada. Sensor desconectado o cables danados. Esperando reset."
#define MSG_NEW_FIRMWARE_AVAILABLE                  				"Nuevo firmware disponible:"
#define MSG_NEW_FIRMWARE_PLEASE_UPGRADE                 			"Actualizar por favor"
#define MSG_FOLLOW_CALIBRATION_FLOW                    			"Impresora no esta calibrada todavia. Por favor usa el manual, capitulo First steps, Calibration flow."
#define MSG_BABYSTEP_Z_NOT_SET                      			"Distancia entre la punta del nozzle y la superficie de la heatbed aun no fijada. Por favor siga el manual, capitulo First steps, First layer calibration."
#define MSG_BED_CORRECTION_MENU                                 "Corr. de la cama"
#define MSG_BED_CORRECTION_LEFT                                 "Izquierda [um]"
#define MSG_BED_CORRECTION_RIGHT                                "Derecha   [um]"
#define MSG_BED_CORRECTION_FRONT                                "Frontal   [um]"
#define MSG_BED_CORRECTION_REAR                                 "Trasera   [um]"
#define MSG_BED_CORRECTION_RESET                                "Reset"

#define MSG_MESH_BED_LEVELING									"Mesh Bed Leveling"
#define MSG_MENU_CALIBRATION									"Calibracion"
#define MSG_TOSHIBA_FLASH_AIR_COMPATIBILITY_OFF					"SD card [normal]"
#define MSG_TOSHIBA_FLASH_AIR_COMPATIBILITY_ON					"SD card [FlshAir]"

#define MSG_LOOSE_PULLEY								"Polea suelta"
#define MSG_FILAMENT_LOADING_T0							"Insertar filamento en el extrusor 1. Haz clic una vez terminado."
#define MSG_FILAMENT_LOADING_T1							"Insertar filamento en el extrusor 2. Haz clic una vez terminado."
#define MSG_FILAMENT_LOADING_T2							"Insertar filamento en el extrusor 3. Haz clic una vez terminado."
#define MSG_FILAMENT_LOADING_T3							"Insertar filamento en el extrusor 4. Haz clic una vez terminado."
#define MSG_CHANGE_EXTR									"Cambiar extrusor."

#define MSG_FIL_ADJUSTING								"Ajustando filamentos. Espera por favor."
#define MSG_CONFIRM_NOZZLE_CLEAN_FIL_ADJ				"Filamentos ajustados. Limpia nozzle para calibracion. Haz clic una vez terminado."
#define MSG_CALIBRATE_E									"Calibrar E"
#define MSG_E_CAL_KNOB									"Rotar el mando hasta que la marca llegue al cuerpo del extrusor. Haz clic una vez terminado."
#define MSG_MARK_FIL									"Marque el filamento 100 mm por encima del final del extrusor. Haz clic una vez terminado."
#define MSG_CLEAN_NOZZLE_E								"E calibrado. Limpia nozzle. Haz clic una vez terminado."
#define MSG_WAITING_TEMP								"Esperando enfriamiento de heatbed y extrusor."
#define MSG_FILAMENT_CLEAN								"Es el nuevo color nitido?"
#define MSG_UNLOADING_FILAMENT							"Soltando filamento"
#define MSG_PAPER										"Colocar una hoja de papel sobre la superficie de impresion durante la calibracion de los primeros 4 puntos. Si la boquilla mueve el papel, apagar impresora inmediatamente."

#define MSG_FINISHING_MOVEMENTS							"Term. movimientos"
#define MSG_PRINT_PAUSED								"Impresion en pausa"
#define MSG_RESUMING_PRINT								"Reanudar impresion"
#define MSG_PID_EXTRUDER								"Calibracion PID"
#define MSG_SET_TEMPERATURE								"Establecer temp.:"
#define MSG_PID_FINISHED								"Cal. PID terminada"
#define MSG_PID_RUNNING									"Cal. PID           "

#define MSG_CALIBRATE_PINDA								"Calibrar"
#define MSG_CALIBRATION_PINDA_MENU						"Calibracion temp."
#define MSG_PINDA_NOT_CALIBRATED						"La temperatura de calibracion no ha sido ajustada"
#define MSG_PINDA_PREHEAT								"Calentando PINDA"
#define MSG_TEMP_CALIBRATION							"Cal. temp.          "
#define MSG_TEMP_CALIBRATION_DONE						"Calibracon temperatura terminada. Haz clic para continuar."
#define MSG_TEMP_CALIBRATION_ON							"Cal. temp. [ON]"
#define MSG_TEMP_CALIBRATION_OFF						"Cal. temp. [OFF]"

#define MSG_PREPARE_FILAMENT							"Preparar filamento"



#define MSG_LOAD_ALL									"Intr. todos fil."
#define MSG_LOAD_FILAMENT_1								"Introducir fil. 1"
#define MSG_LOAD_FILAMENT_2								"Introducir fil. 2"
#define MSG_LOAD_FILAMENT_3								"Introducir fil. 3"
#define MSG_LOAD_FILAMENT_4								"Introducir fil. 4"
#define MSG_UNLOAD_FILAMENT_1							"Soltar fil. 1"
#define MSG_UNLOAD_FILAMENT_2							"Soltar fil. 2"
#define MSG_UNLOAD_FILAMENT_3							"Soltar fil. 3"
#define MSG_UNLOAD_FILAMENT_4							"Soltar fil. 4"
#define MSG_UNLOAD_ALL									"Soltar todos fil."
#define MSG_PREPARE_FILAMENT							"Preparar filamento"
#define MSG_ALL											"Todos"
#define MSG_USED										"Usado en impresion"
#define MSG_CURRENT										"Actual"
#define MSG_CHOOSE_EXTRUDER								"Elegir extrusor:"
#define MSG_EXTRUDER									"Extrusor"
#define MSG_EXTRUDER_1									"Extrusor 1"
#define MSG_EXTRUDER_2									"Extrusor 2"
#define MSG_EXTRUDER_3									"Extrusor 3"
#define MSG_EXTRUDER_4									"Extrusor 4"
#define MSG_DATE							"Fecha:"
#define MSG_XYZ_DETAILS						"Inform. XYZ cal."
#define	MSG_Y_DISTANCE_FROM_MIN				"Dist. Y desde min:"
#define	MSG_LEFT							"Izquierda:"
#define MSG_CENTER							"Centro:"
#define MSG_RIGHT							"Derecha:"
#define MSG_MEASURED_SKEW					"Inclin. medida:"
#define MSG_SLIGHT_SKEW						"Inclin. ligera:"
#define MSG_SEVERE_SKEW						"Inclin. severa:"
#define MSG_SORT_TIME						"Ordenar: [Tiempo]"
#define MSG_SORT_ALPHA						"Ordenar:[Alfabet]"
#define MSG_SORT_NONE						"Ordenar:[Ninguno]"
#define MSG_WIZARD							"Wizard"
#define MSG_DEFAULT_SETTINGS_LOADED			"Ajustes por defecto cargados."
#define MSG_SORTING							"Ordenando archivos"
#define MSG_FILE_INCOMPLETE					"Archivo imcompleto. Deseas continuar?"
#define MSG_WIZARD_WELCOME					"Hola, soy tu impresora Unoriginal Prusa i3. Quieres que te guie a traves de la configuracion?"
#define MSG_WIZARD_QUIT						"Siempre puedes acceder al asistente desde Calibracion -> Wizard"
#define MSG_WIZARD_SELFTEST					"Primero, hare el Selftest para comprobar los problemas de montaje mas comunes."
#define MSG_WIZARD_CALIBRATION_FAILED		"Lee el manual y resuelve el problema. Despues, reinicia la impresora y continua con el Wizard"
#define MSG_WIZARD_XYZ_CAL					"Hare la calibracion XYZ. Tardara 12 min. aproximadamente."
#define MSG_WIZARD_FILAMENT_LOADED			"Esta el filamento cargado?"
#define MSG_WIZARD_Z_CAL					"Voy a hacer Calibracion Z ahora."
#define MSG_WIZARD_WILL_PREHEAT				"Voy a precalentar el nozzle para PLA ahora."
#define MSG_WIZARD_V2_CAL					"Voy a calibrar la distancia entre la punta del nozzle y la superficie de la heatbed."
#define MSG_WIZARD_V2_CAL_2					"Voy a comenzar a imprimir la linea y tu bajaras el nozzle gradualmente al rotar el mando, hasta que llegues a la altura optima. Mira las imagenes del capitulo Calibracion en el manual."
#define MSG_V2_CALIBRATION					"Cal. primera cap."
#define MSG_WIZARD_DONE						"Terminado! Feliz impresion!"
#define MSG_WIZARD_LOAD_FILAMENT			"Inserta, por favor, filamento PLA en el extrusor. Despues haz clic para cargarlo."
#define MSG_WIZARD_RERUN					"Ejecutar el Wizard borrara los valores de calibracion actuales y comenzara de nuevo. Continuar?"
#define MSG_WIZARD_REPEAT_V2_CAL			"Quieres repetir el ultimo paso para reajustar la distancia nozzle-heatbed?"
#define MSG_WIZARD_CLEAN_HEATBED			"Limpia la superficie de la heatbed, por favor, y haz clic"
#define MSG_WIZARD_PLA_FILAMENT				"Es el filamento PLA?"
#define MSG_WIZARD_INSERT_CORRECT_FILAMENT	"Carga filamento PLA, por favor, y reinicia la impresora para continuar con el Wizard"
#define MSG_PLA_FILAMENT_LOADED				"Esta el filamento PLA cargado?"
#define MSG_PLEASE_LOAD_PLA					"Carga el filamento PLA primero por favor."
#define MSG_FILE_CNT						"Algunos archivos no seran ordenados. El Max. num. de archivos para ordenar en 1 carpeta es 100."
#define MSG_WIZARD_HEATING					"Precalentando nozzle. Espera por favor."
#define MSG_M117_V2_CALIBRATION				"M117 Cal. primera cap."

