var express = require('express');
var router = express.Router();
const db = require('./sql.js');//引入MySQL模块

router.get('/init', (req, res, next) => {
    db.query('DELETE FROM records;', (err, result) => {
        next();
    });
}, (req, res, next) => {
    res.redirect(301, '/#tab1=111');
});
//增
router.post('/insert_user', (req, res, next) => {
    db.query('INSERT INTO members (stuid,name,cardid,type) VALUES (?,?,?,?);', [req.body.insertstuid, req.body.insertname, req.body.insertcardid, req.body.inserttype], function (err, result) {
        if (err == null) {
            res.send('ok');
        } else {
            console.log('insert user failed, the reseason is ' + err);
            res.send('error');
        }
    });
});

router.post('/insert_room', (req, res, next) => {
    db.query('INSERT INTO rooms (num,mac) VALUES (?,?);', [req.body.insertnum, req.body.insertmac], function (err, result) {
        if (err == null) {
            res.send('ok');
        } else {
            console.log('insert user failed, the reseason is ' + err);
            res.send('error');
        }
    });
});

router.post('/insert_admin', (req, res, next) => {
    db.query('INSERT INTO admins (username,password) VALUES (?,?);', [req.body.insertusername, req.body.insertpassword], function (err, result) {
        if (err == null) {
            res.send('ok');
        } else {
            console.log('insert user failed, the reseason is ' + err);
            res.send('error');
        }
    });
});
//删
router.post('/delete_users', (req, res, next) => {
    var id = parseInt(req.body.id);
    db.query('DELETE FROM members WHERE id = ?', id, function (err, result) {
        if (err) {
            console.log(err);
        } else {
            res.send({ 'status': 'success' });
        }
    });
});

router.post('/delete_rooms', (req, res, next) => {
    var id = parseInt(req.body.id);
    db.query('DELETE FROM rooms WHERE id = ?', id, function (err, result) {
        if (err) {
            console.log(err);
        } else {
            res.send({ 'status': 'success' });
        }
    });
});

router.post('/delete_admins', (req, res, next) => {
    var id = parseInt(req.body.id);
    db.query('DELETE FROM admins WHERE id = ?', id, function (err, result) {
        if (err) {
            console.log(err);
        } else {
            res.send({ 'status': 'success' });
        }
    });
});
//改
router.post('/update_users', (req, res, next) => {
      var stuid = req.body.stuid;
      var name = req.body.name;
      var cardid = req.body.cardid;
      console.log("stuid: " + stuid);
      console.log("name: " + name);
      console.log("cardid: ", cardid);

    if (!stuid || !name || !cardid) {
        res.send({ 'status': 'failure', 'message': '输入不能为空。' });
        return;
      }
      const illegalCharsRegex = /[!@#$%^&*()_+{}\[\]:;"'<>,.?\\/|·！@#￥%……&*（）——+{}【】‘；：”“’《》，。？、｜\s【】？“”‘’！……【】《》（）；：。，、？【】{}——“”‘’——【】——{}【】【】【】]/;
    
    
      // 检查输入是否包含非法字符
      if (illegalCharsRegex.test(stuid) || illegalCharsRegex.test(name) || illegalCharsRegex.test(cardid)) {
        res.send({ 'status': 'failure', 'message': '输入包含非法字符。' });
        return;
      }
    
      db.query('UPDATE members SET name = ?, cardid = ? WHERE stuid = ?', [name, cardid, stuid], (err, result) => {
        if (err) {
          console.error(err);
          res.send({ 'status': 'failure', 'message': '更新失败。' });
        } else {
          res.send({ 'status': 'success' });
        }
      });
    });

router.post('/update_rooms', (req, res, next) => {
    db.query('UPDATE rooms SET mac = ? WHERE num =?', [req.body.mac, req.body.num]);
    res.send({ 'status': 'success' });
});


router.post('/update_admins', (req, res, next) => {
    db.query('SELECT * FROM admins WHERE username = ?;', req.body.oldusername, (err, row) => {//管理员旧的用户名
        if (req.body.oldpassword == row[0].password) {//旧密码与管理的是否匹配
            if (req.body.newpassword == '') {
                db.query('UPDATE admins SET username = ? WHERE id = ?', [req.body.newusername, req.body.id]);
                res.send({ 'status': 'success', 'sign': 0 });
            }
            else {
                db.query('UPDATE admins SET username = ?, password = ? WHERE id = ?', [req.body.newusername, req.body.newpassword, req.body.id]);
                res.send({ 'status': 'success', 'sign': 0 });//执行 SQL 更新语句，将新用户名更新到 admins 表中。
            }
        }
        else {
            res.send({ 'status': 'success', 'sign': 1 });
        }
    });
});


module.exports = router;