function getResultFromClient(data_to_send) {
    // 创建WebSocket连接
    const socket = new WebSocket('ws://100.79.220.28:3000');
    // 创建Promise对象用于处理异步操作
    return new Promise((resolve, reject) => {
        // 监听连接成功事件
        socket.addEventListener('open', (event) => {
            console.log('Connected to server');

            // 向服务器发送数据
            console.log(`Sending: ${data_to_send}`);
            socket.send(data_to_send);
        });

        // 监听接收消息事件
        socket.addEventListener('message', (event) => {
            console.log(`Received from server: ${event.data}`);

            // 解析服务器返回的数据，并将其传递给resolve
            resolve(event.data);

            // 关闭WebSocket连接
            socket.close();
        });

        // 监听连接关闭事件
        socket.addEventListener('close', (event) => {
            console.log('Connection closed');

            // 如果连接关闭时仍未收到数据，拒绝Promise
            reject(new Error('Connection closed without receiving data.'));
        });
    });
};