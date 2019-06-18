import toastr from 'toastr'
import '../node_modules/toastr/build/toastr.css'
import '../node_modules/bootstrap/dist/css/bootstrap.min.css'
import '@ivyknob/avionics_js'
import RModal from 'rmodal'
import modalTemplate from './modal.html'

global.toastr = toastr;
global.modalTemplate = modalTemplate;
global.RModal = RModal;
