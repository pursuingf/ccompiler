Vue.config.productionTip = false;

Vue.component('start', {
    template: '#start'
});

Vue.component('compiler', {
    template: '#compiler',
    data() {
        return {
            form: null,
            editor: null,
            procedure: '',
            lexerTable: 'history',
            lexicalAnalysisResult: null,
            grammarTable: 'lr',
            grammaticalAnalysisResult: null,
        }
    },
    mounted() {
        //编辑器
        this.editor = ace.edit("ace-editor");
        this.editor.setTheme("ace/theme/merbivore_soft");
        this.editor.session.setMode("ace/mode/c_cpp");
        this.editor.setFontSize(18);

        // 处理input 
        const checkFormat = (fileName) => {
            let suffix = fileName.substr(fileName.lastIndexOf('.') + 1).toLowerCase();
            let fileFormat = ['txt', 'cpp'];
            //看看这个后缀存不存在于file_format数组当中
            return fileFormat.includes(suffix);
        }
        const handleInput = (event) => {
            const input = event.target;
            const cover = document.getElementById('file-input-cover');

            this.procedure = "";
            if (input.files.length === 0) {
                cover.textContent = "No file selected.";
                return;
            }
            const fileName = input.files[0].name;
            if (!checkFormat(fileName)) {
                cover.textContent = "Error: CPP/TXT.";
                return;
            }
            cover.innerHTML = fileName.substr(fileName.lastIndexOf('/') + 1);

            const reader = new FileReader();
            const vm = this;
            reader.onload = function () {
                vm.procedure = reader.result;
                vm.editor.setValue(vm.procedure);
            }
            reader.readAsText(input.files[0])
        };

        document.getElementById('file-input').addEventListener("change", handleInput);
    },
    methods: {
        dataToDom(formData) {
            if (this.form == 'lexer') {
                this.lexerTable = formData;
                drawTable(this.lexicalAnalysisResult[formData]['head'], this.lexicalAnalysisResult[formData]['data'], 'canvas');
            }
            else if (this.form == 'grammar') {
                this.grammarTable = formData;
                if (formData == 'tree') {
                    drawTree(this.grammaticalAnalysisResult[formData], 'canvas');
                }
                else {
                    drawTable(this.grammaticalAnalysisResult[formData]['head'], this.grammaticalAnalysisResult[formData]['data'], 'canvas');
                }
            }
        },
        handleSelect() {
            document.getElementById('file-input').click()
        },
        lexicalAnalysis() {
            this.procedure = this.editor.getValue();
            if (this.procedure == '') {
                alert("请检查输入是否为空")
                return;
            }
            this.form = 'lexer';
            getResultFromClient('0' + this.procedure)
                .then((serverResponse) => {
                    console.log(`Server response: ${serverResponse}`);
                    const data = JSON.parse(serverResponse);
                    this.lexicalAnalysisResult = {
                        'history': { 'head': ['单词类别', '单词值'], 'data': data[1] },
                        'name': { 'head': ['ID', '符号名'], 'data': data[0] }
                    }
                    this.dataToDom(this.lexerTable);
                })
                .catch((error) => {
                    console.error(`Error: ${error.message}`);
                    this.form = null;
                    alert("分析失败");
                    return;
                });
            console.log(this.procedure)
        },
        grammaticalAnalysis() {
            this.procedure = this.editor.getValue();
            if (this.procedure == '') {
                alert("请检查输入是否为空")
                return;
            }
            this.form = 'grammar';
            getResultFromClient('1' + this.procedure)
                .then((serverResponse) => {
                    console.log(`Server response: ${serverResponse}`);
                    const data = JSON.parse(serverResponse);
                    this.grammaticalAnalysisResult = {
                        'lr': { 'head': data[0][0], 'data': data[0].slice(1) },
                        'process': { 'head': ['序号', '状态', '符号', '输入串'], 'data': data[1].map((element, index) => [index + 1, ...element]) },
                        'tree': processData(data[2])
                    }
                    this.dataToDom(this.grammarTable);
                })
                .catch((error) => {
                    console.error(`Error: ${error.message}`);
                    this.form = null;
                    alert("分析失败");
                    return;
                });
            console.log(this.procedure)
        }
    }
});

Vue.component('lexer', {
    template: '#lexer',
    data() {
        return {
            table: 'history',
        }
    }
})

Vue.component('grammar', {
    template: '#grammar',
    data() {
        return {
            table: 'lr',
        }
    }
})

new Vue({
    el: '#app',

    data: {
        page: 'start',
    },

    methods: {
        onChangePage(to) {
            this.page = to;
        },
    },
});

