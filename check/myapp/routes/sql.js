var express = require('express');
var router = express.Router();
var mysql = require('mysql');

/* GET home page. */
router.get('/', function (req, res, next) {
    res.render('index', { title: 'Express' });
});

const connection = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: '123456', /* 123456 */
    database: 'test'
});

connection.connect(function (err) {
    if (err) {
        console.error('连接错误: ' + err.stack);
        return;
    }
    console.log('连接ID：' + connection.threadId);  
});

module.exports = connection;