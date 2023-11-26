/**
 * 树数据处理
 * @param {Array} data 语法树的先序遍历数组
 * @returns 符合Echarts输入格式的data
 */
const processData = (data) => {
    let index = 0;

    function buildTree(n) {
        let node = {
            name: data[index][1],
            children: [],
            value: data[index][2]
        };

        while (index + 1 < data.length && parseInt(data[index + 1][0]) > parseInt(data[n][0])) {
            index++;
            let childNode = buildTree(index);
            node.children.push(childNode);
        }

        return node;
    }

    return buildTree(0);
}

/**
 * 树形展示数据
 * @param {*} data name和children[]层层组成
 * @param {string} nodeId 挂载节点id
 */
const drawTree = (data, nodeId) => {
    let element = document.getElementById(nodeId);
    element.innerHTML = '';
    const myEcharts = echarts.init(element);

    const options = {
        tooltip: {//提示框
            trigger: "item",//触发时机
        },
        series: [
            {
                type: "tree",//树图
                roam: true,
                scaleLimit: {
                    min: 0.5,
                    max: 3
                },
                orient: 'TB',
                data: [data],
                top: "10%",//tree组件离容器顶部的距离
                left: "5%",
                bottom: "5%",
                right: "5%",
                symbolSize: 30,//标记的大小
                label: {//描述了每个节点所对应的文本标签的样式。
                    position: "left",//标签的位置。
                    verticalAlign: "middle",//文字垂直对齐方式
                    align: "right",//文字水平对齐方式
                    fontSize: 9,
                },
                leaves: {//叶子节点的特殊配置
                    label: {//了叶子节点所对应的文本标签的样式
                        position: "right",
                        verticalAlign: "middle",
                        align: "left",
                    },
                },
                emphasis: {//树图中个图形和标签高亮的样式。
                    focus: "descendant",//聚焦所有子孙节点
                },
                expandAndCollapse: true,//子树折叠和展开的交互由于绘图区域是有限的，而通常一个树图的节点可能会比较多，
                // 这样就会出现节点之间相互遮盖的问题。为了避免这一问题，可以将暂时无关的子树折叠收起，
                // 等到需要时再将其展开。如上面径向布局树图示例，节点中心用蓝色填充的就是折叠收起的子树，
                // 可以点击将其展开。
                animationDuration: 550,
                animationDurationUpdate: 750,
            },
        ],
    };

    myEcharts.setOption(options);
}


/**
 * 绘制表格
 * @param {Array} head 表项
 * @param {Array} data 表格数据
 * @param {string} nodeId 挂载节点id
 */
const drawTable = (head, data, nodeId) => {
    let result = `<table><thead><tr>`;
    for (let i = 0; i < head.length; ++i) {
        result += (`<th>` + head[i] + `</th>`);
    }
    result += `</tr></thead>`

    result += `<tbody>`;
    for (let i = 0; i < data.length; ++i) {
        result += `<tr>`;
        for (let j = 0; j < head.length; ++j) {
            result += `<td>${data[i][j]}</td>`;
        }
        result += `</tr>`;
    }
    result += '</tbody></table>';

    let element = document.getElementById(nodeId);
    element.innerHTML = '';
    const doc = new DOMParser().parseFromString(result, 'text/html');
    while (doc.body.firstChild) {
        element.appendChild(doc.body.firstChild);
    }
}