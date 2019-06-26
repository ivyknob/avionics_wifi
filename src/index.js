/* global global */
// Заменить на yarn add toastify-js
import toastr from 'toastr'

import 'bootstrap/dist/css/bootstrap.min.css'
import 'toastr/build/toastr.css'
import '@ivyknob/avionics_js'
import RModal from 'rmodal'
import modalTemplate from './modal.html'

global.toastr = toastr;
global.modalTemplate = modalTemplate;
global.RModal = RModal;
