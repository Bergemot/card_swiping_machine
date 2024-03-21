var express = require('express');
var router = express.Router();
const db = require('./sql.js');//连接数据库
const EventEmitter = require('events'); //引入事件模块
var flag = 0;
var flag1 = 0;
var mac1;
var attend;
var f;
var bt = '1970-01-01 00:00:00';
var et = '2100-01-01 00:00:00';
var qdt = "";
var mem = "%";
//////////////////////////
let cla = "%";
var rom = "%";
var adm = "%";
var normal = 0;
var late = 0;

const myEmitter = new EventEmitter.EventEmitter();//创建EventEmitter对象用于监听
//注册seen事件用于监视

myEmitter.on('seen', () => {//签到成功
    flag = 1;
});
myEmitter.on('seen1', () => {//重复刷卡
    flag = 2;
});
//创建具体路由
router.get('/check', function (req, res) {//当收到带有路径‘/check'的get请求时，将返回send（）
    res.send({ 'status': 'success', 'flag': flag, 'id': attend, 'flag1': flag1});
    flag = 0;
    flag1 = 1;
});

router.get('/chartcheck', function (req, res) {
    res.send({ 'status': 'success', 'la': late, 'no': normal });
});

router.post('/', (req, res, next) => {       //HTTP 请求对象、HTTP 响应、中间件链中的下一个函数。
    console.log(req.body.mac);
    var aaa = '%' + req.body.id + '%';
    db.query('SELECT * FROM members WHERE cardid LIKE ?;', aaa, (err, rows) => {
        if (rows[0]) {
            var bbb = '%' + req.body.mac + '%';
            db.query('SELECT * FROM rooms WHERE mac LIKE ?;', bbb, (err, rooms) => {
                if (rooms == "") {
                    let user = {
                        result: "failed",
                        mes: "",
                        reason: "wrong room"
                    }
                    console.log(user);
                    res.send(user);
                }
                else {
                    db.query('SELECT * FROM types WHERE num=?;', rows[0].type, (err, types) => {
                        
                            f = rows[0].stuid;
                            mac1 = rooms[0].num;
                            var date = new Date();
                            var year = date.getFullYear();
                            var month = date.getMonth() + 1;
                            month = month < 10 ? ('0' + month) : month;
                            var day = date.getDate();
                            day = day < 10 ? ('0' + day) : day;
                            var hour = date.getHours();
                            hour = hour < 10 ? ('0' + hour) : hour;
                            var minute = date.getMinutes();
                            minute = minute < 10 ? ('0' + minute) : minute;
                            var second = date.getSeconds();
                            second = second < 10 ? ('0' + second) : second;
                            var t0 = hour + ':' + minute + ':' + second;
                            var qd;
                            if (qdt != "") {
                                if (t0 > qdt) qd = '迟到';
                                else qd = '正常';
                            }
                            else qd = '正常';
                            var t = year + '-' + month + '-' + day + ' ' + hour + ':' + minute + ':' + second;
                            db.query('INSERT INTO records (stuid,name,type,room,time,qd) VALUES (?,?,?,?,?,?);', [rows[0].stuid, rows[0].name, types[0].key, rooms[0].num, t, qd], function (err, result) {
                                let user = {
                                    result: "success",
                                    idcard: rows[0].stuid,
                                    name: rows[0].name
                                }
                                console.log(user);
                                res.send(user);
                                attend = user.idcard + " " + user.name;
                                myEmitter.emit('seen');
                            });
                        
                    });
                }
            });
        }
        else {
            let user = {
                result: "failed",
                mes: req.body.id,
                reason: "not found"
            }
            console.log(user);
            res.send(user);
        }
    });
});

router.post('/login', (req, res, next) => {
    db.query('SELECT * FROM admins WHERE username = ?;', req.body.usernameinput, (err, row) => {
        if (row != "") {
            if (req.body.passwordinput == row[0].password) {
                res.send({ 'status': 'success', 'sign': 0 });
            }
            else {
                res.send({ 'status': 'success', 'sign': 1 });
            }
        }
        else {
            res.send({ 'status': 'success', 'sign': 2 });
        }
    });
});

router.post('/timesearch', (req, res, next) => {
    if (req.body.sign == 0) {
        bt = '1970-1-1 00:00:00';
        et = '2100-1-1 00:00:00';
    }
    else {
        bt = req.body.bt;
        et = req.body.et;
    }
    res.send({ 'status': 'success', 'la': late, 'no': normal });
});

router.post('/qdtime', (req, res, next) => {//签到时间
    if (req.body.sign == 0) {
        qdt = "";
    }
    else {
        qdt = req.body.time;
    }
    res.send({ 'status': 'success'});
});

router.post('/idsearch', (req, res, next) => {
    if (req.body.sign == 0) {
        mem = "%";
    }
    else {
        mem = "%" + req.body.id + "%";
    }
    res.send({ 'status': 'success' });
});
/////////////////////////////////////////////////////////////////
router.post('/rosearch', (req, res, next) => {
    if (req.body.sign == 0) {
        cla = "%";
    }
    else {
        cla = "%" + req.body.id + "%";
    }
    res.send({ 'status': 'success' });
});

router.post('/roomsearch', (req, res, next) => {
    if (req.body.sign == 0) {
        rom = "%";
    }
    else {
        rom = "%" + req.body.id + "%";
    }
    res.send({ 'status': 'success' });
});

router.post('/adminsearch', (req, res, next) => {
    if (req.body.sign == 0) {
        adm = "%";
    }
    else {
        adm = "%" + req.body.id + "%";
    }
    res.send({ 'status': 'success' });
});
//检查唯一性
router.post('/inputcheck0', (req, res, next) => {
    db.query('SELECT * FROM members WHERE stuid = ?;', req.body.idinput, (err, row) => {
        if (row != "") {
            res.send({ 'status': 'success', 'sign': 0 });
        }
        else res.send({ 'status': 'success', 'sign': 1 });
    });
});

router.post('/inputcheck1', (req, res, next) => {
    var idinput = "%" + req.body.idinput + ";%";
    db.query('SELECT * FROM members WHERE cardid LIKE ? OR cardid = ?;', [idinput, req.body.idinput], (err, row) => {
        if (row != "") {
            res.send({ 'status': 'success', 'sign': 0 });
        }
        else res.send({ 'status': 'success', 'sign': 1 });
    });
});

router.post('/inputcheck2', (req, res, next) => {
    db.query('SELECT * FROM rooms WHERE num = ?;', req.body.idinput, (err, row) => {
        if (row != "") {
            res.send({ 'status': 'success', 'sign': 0 });
        }
        else res.send({ 'status': 'success', 'sign': 1 });
    });
});

router.post('/inputcheck3', (req, res, next) => {
    var idinput = "%" + req.body.idinput + ";%";
    db.query('SELECT * FROM rooms WHERE mac LIKE ? OR mac = ?;', [idinput, req.body.idinput], (err, row) => {
        if (row != "") {
            res.send({ 'status': 'success', 'sign': 0 });
        }
        else res.send({ 'status': 'success', 'sign': 1 });
    });
});

router.post('/inputcheck4', (req, res, next) => {
    db.query('SELECT * FROM admins WHERE username = ?;', req.body.usernameinput, (err, row) => {
        if (row != "") {
            res.send({ 'status': 'success', 'sign': 0 });
        }
        else res.send({ 'status': 'success', 'sign': 1 });
    });
});

////////////////////////////////////////////////////////////////////////////////////////////////////////////
const moment = require('moment'); // 引入moment库，用于处理日期和时间

router.post('/duplicate_check', (req, res, next) => {
    db.query('SELECT * FROM records WHERE stuid = ?;', req.body.id, (err, row) => {
        if (err) {
            // 处理数据库查询错误
            console.error(err);
            res.status(500).send({ 'status': 'error', 'message': 'An error occurred' });
            return;
        }

        if (row.length > 0) {
            let result = row[row.length-1]; // 获取最后一条查询结果
            console.log("Received result:", result);
            let record_time = moment(result.time); // 获取该条记录的打卡时间并用moment解析

            let nowtime = moment(); // 获取当前时间

            // 计算时间差，单位为分钟
            let timeDifference = nowtime.diff(record_time, 'minutes');

            if (timeDifference < 5) {
                res.send({ 'status': 'failed', 'sign': 0 });
            } else {
                res.send({ 'status': 'success', 'sign': 1 });
            }
        } else {
            res.send({ 'status': 'success', 'sign': 1 });
        }
    });
});

router.post('/stuid_check', (req, res, next) => {
    db.query('SELECT * FROM members WHERE cardid = ?;', req.body.id, (err, row) => {
        if (err) {
            // 处理数据库查询错误
            console.error(err);
            res.status(500).send({ 'status': 'error', 'message': 'An error occurred' });
            return;
        }
        if (row.length > 0) {
            let result = row[0]; // 获取第一条查询结果
            console.log("Received result:", result);
            let studentid = result.stuid; 

                res.send({ 'status': 'success', 'sign': 1 ,'stuid': studentid});
            }
         else {
            res.send({ 'status': 'failed', 'sign': 1 });
        }
    });
});
///////////////////////////////////////////////////////////////////////////////////////liu
router.get('/list', (req, res, next) => {//list?
    db.query('SELECT * FROM members WHERE stuid LIKE ? ORDER BY id DESC;', mem, (err, rows) => {
        let rets = [];
        if (rows) {
            rows.forEach(row => {
                let user = {
                    id: row.id,
                    stuid: row.stuid,
                    name: row.name,
                    cardid: row.cardid,
                    type: parseInt(row.type)
                }
                rets.push(user);
            });
            let retdata = {
                code: 0,
                msg: "",
                count: 10,
                data: rets
            }
            res.send(JSON.stringify(retdata));
        }
    });
});

router.get('/recordlist', (req, res, next) => {
    late = 0; normal = 0;
    sw = 'SELECT * FROM records WHERE time BETWEEN \'' + bt + '\' AND \'' + et + '\' ORDER BY time DESC;'
    db.query(sw, (err, rows) => {
        let rets = [];
        rows.forEach(row => {
            let user = {
                id: row.id,
                stuid: row.stuid,
                name: row.name,
                type: row.type,
                room: row.room,
                time: row.time,
                qd: row.qd
            }
            if (user.qd == '迟到') {
                late = late + 1;
            }
            else normal = normal + 1;
            rets.push(user);
        });
        let retdata = {
            code: 0,
            msg: "",
            count: 10,
            data: rets
        }
        res.send(JSON.stringify(retdata));
    });
});

router.get('/roomslist', (req, res, next) => {
    db.query('SELECT * FROM rooms WHERE num LIKE ? ORDER BY id DESC;', rom, (err, rows) => {
        let rets = [];
        rows.forEach(row => {
            let user = {
                id: row.id,
                num: row.num,
                mac: row.mac
            }
            rets.push(user);
        });
        let retdata = {
            code: 0,
            msg: "",
            count: 10,
            data: rets
        }
        res.send(JSON.stringify(retdata));
    });
});
//////////////////////////////////////////////////////////////
router.get('/rolist', (req, res, next) => {
    db.query('SELECT * FROM records WHERE room LIKE ? ORDER BY id DESC;', cla, (err, rows) => {
        let rets = [];
        rows.forEach(row => {
            let user = {
                id: row.id, 
                stuid: row.stuid,
                name: row.name,
                type: row.type,
                room: row.room,
                time: row.time,
                qd: row.qd
            }
            rets.push(user);
        });
        let retdata = {
            code: 0,
            msg: "",
            count: 10,
            data: rets
        }
        res.send(JSON.stringify(retdata));
    });
});

router.get('/adminslist', (req, res, next) => {
    db.query('SELECT * FROM admins WHERE username LIKE ? ORDER BY id DESC;', adm, (err, rows) => {
        let rets = [];
        rows.forEach(row => {
            let user = {
                id: row.id,
                username: row.username,
                password: row.password
            }
            rets.push(user);
        });
        let retdata = {
            code: 0,
            msg: "",
            count: 10,
            data: rets
        }
        res.send(JSON.stringify(retdata));
    });
});
////////////////////////////////////////////////////////////////////////
router.get('/current-time', function(req, res) {
    
    // 获取当前时间
    var currentTime = new Date().toLocaleString();
  
    // 构建响应对象
    var response = {
      status: 'success',
      currentTime: currentTime
    };
    res.setHeader('X-Powered-By', '');

    // 发送响应
    res.send(response);
  });

router.post('/get_roomnum', (req, res, next) => {
    var mac = req.body.mac; // 获取客户端发送的mac参数
    console.log("Received POST request with mac:", mac);
    var idinput = "%" + req.body.idinput + ";%";
    console.log("Received POST request with idinput:", req.body.idinput);

    // 在控制台打印出接收到的MAC地址信息
    console.log("Received MAC address:", idinput);
    db.query('SELECT * FROM rooms WHERE mac LIKE ? OR mac = ?;', [mac, req.body.mac], (err, row) => {
        if (row != "") {
            // 数据库中找到了匹配的数据
            let result = row[0]; // 假设只取第一条匹配的数据
            let retdata = {
                status: 'success',
                sign: 0,
                num: result.num // 将查找到的num加入到响应中
            };
            res.send(retdata);
        } else {
            // 数据库中未找到匹配的数据
            res.send({
                status: 'success',
                sign: 1
            });
        }
    });
});
/////////////////////////////////////////////////////////////////////////////////////////////

module.exports = router;
