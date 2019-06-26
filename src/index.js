/* global global */
import 'bootstrap/dist/css/bootstrap.min.css'
import Toastify from 'toastify-js'
import "toastify-js/src/toastify.css"
import '@ivyknob/avionics_js'
import RModal from 'rmodal'
import modalTemplate from './modal.html'

global.Toastify = Toastify;
global.modalTemplate = modalTemplate;
global.RModal = RModal;
