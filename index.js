var express = require('express');
var app = express();
var bodyParser = require('body-parser');
const { type } = require('os');

app.use(express.static('public'));
app.set('view engine', 'jade');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

var types = [
    {
      title: "balance",
      description: "Ejecuta un codigo de c++"
    },
];

types.forEach(function (type) {
    app.use('/'+type.title, require('./routes/' + type.title));
});
   
