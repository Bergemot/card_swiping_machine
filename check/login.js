const express = require('express');
const mysql = require('mysql');

const app = express();
const port = 2000;

const bodyParser = require('body-parser');

// 创建数据库连接
const connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '123456',
  database: 'login'
});

// 连接数据库
connection.connect((error) => {
  if (error) {
    console.error('Error connecting to database:', error);
    return;
  }
  console.log('Connected to database!');
});

app.get('/', function(req, res){
    res.sendFile(__dirname + '/login.html');
  });

// 解析请求体中的JSON数据
app.use(express.json());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false}))

// 处理登录请求
app.post('/login', (req, res) => {
  console.log('Raw Request Data:', JSON.stringify(req.body));
  const { username, password } = req.body;
  // console.log('Received login request:', { username, password });
  // 在数据库中查询用户记录
  connection.query(`SELECT * FROM login WHERE username = ? AND password = ?`, [username, password], (error, results) => {
    if (error) {
      console.error('Error querying database:', error);
      res.sendStatus(500);
      return;
    }

    if (results.length === 0) {
      // 用户名或密码不正确
      res.send({
        code: 1,
        message: 'Invalid username or password'
      });
    } else {
      // 登录成功
      res.send({
        code: 0,
        message: 'Success'
      });
    }
  });
});

// 启动服务器
app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});
