router.post('/', function(req, res) {
    var execFile = require('child_process').execFile
    // we build this with node-gyp above...
    var program = "../build/Release/standalone";

});