var _component;
var _source;
var _parent;
var _properties;

function create(source, parent, properties) {
    _source = source;
    _parent = parent;
    _properties = properties;

    _component = Qt.createComponent(source);
    if (_component.status === Component.Ready ||
            _component.status === Component.Error) {
        createDone();
    } else {
        _component.statusChanged.connect(createDone);
    }
}

function createDone() {
    if (_component.status === Component.Ready) {
        var object = _component.createObject(_parent, _properties);
        if (object !== null) {
            return object;
        } else {
            console.log("Error creating object: " + _source);
        }

        _component.destroy();
    } else if (_component.status === Component.Error) {
        console.log("Error creating object: " + _component.errorString());
    }
}
