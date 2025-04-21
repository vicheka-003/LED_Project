<script>
    // Update the clock
    function updateClock() {
        const now = new Date();
        const timeElement = document.getElementById('time');
        timeElement.textContent = now.toLocaleTimeString();
    }
    setInterval(updateClock, 1000);
    updateClock();

    // Connection status simulation
    function updateConnectionStatus() {
        const statusDot = document.getElementById('connection-status');
        const statusText = document.getElementById('status-text');
        const isConnected = {% if message and 'failed' in message %}false{% else %}true{% endif %};
        
        if (isConnected) {
            statusDot.classList.remove('disconnected');
            statusDot.classList.add('connected');
            statusText.textContent = 'Connected to Arduino';
        } else {
            statusDot.classList.remove('connected');
            statusDot.classList.add('disconnected');
            statusText.textContent = 'Disconnected';
        }
    }
    updateConnectionStatus();

    // Update LED status based on URL parameter or previous state
    function updateLEDStatus() {
        const urlParams = new URLSearchParams(window.location.search);
        const command = urlParams.get('last_command');
        
        // First, load states from localStorage if available
        for (let i = 1; i <= 3; i++) {
            const savedState = localStorage.getItem(led${i}State);
            if (savedState) {
                updateLEDVisual(i, savedState === 'true');
            }
        }
        
        // Then apply the latest command if present
        if (command) {
            const led = command.slice(-1);
            const action = command.slice(0, -1);
            const isOn = (action === 'on');
            
            // Update the state
            updateLEDVisual(led, isOn);
            
            // Save state to localStorage
            localStorage.setItem(led${led}State, isOn);
            
            document.getElementById('last-command').textContent = 
                Last action: LED ${led} turned ${isOn ? 'ON' : 'OFF'};
        }
    }
    
    // Helper function to update LED visual state
    function updateLEDVisual(ledNumber, isOn) {
        const statusElement = document.getElementById(led${ledNumber}-status);
        const visualElement = document.getElementById(led${ledNumber}-visual);
        
        if (isOn) {
            statusElement.textContent = 'ON';
            statusElement.classList.remove('off');
            statusElement.classList.add('on');
            visualElement.classList.add(on-${ledNumber});
        } else {
            statusElement.textContent = 'OFF';
            statusElement.classList.remove('on');
            statusElement.classList.add('off');
            visualElement.classList.remove(on-${ledNumber});
        }
    }
    updateLEDStatus();

    // Process message display
    function processMessage() {
        const messageDisplay = document.getElementById('message-display');
        if (messageDisplay) {
            setTimeout(() => {
                messageDisplay.style.display = 'none';
            }, 5000);
        }
    }
    processMessage();

    // Show notification
    function showNotification(message, type = 'info') {
        const container = document.getElementById('message-container');
        const notification = document.createElement('div');
        notification.className = message message-${type};
        
        let icon = 'info-circle';
        if (type === 'success') icon = 'check-circle';
        if (type === 'error') icon = 'exclamation-circle';
        
        notification.innerHTML = `
            <i class="fas fa-${icon}"></i>
            <span>${message}</span>
        `;
        
        container.appendChild(notification);
        
        setTimeout(() => {
            notification.style.opacity = '0';
            notification.style.transform = 'translateX(100%)';
            setTimeout(() => {
                container.removeChild(notification);
            }, 300);
        }, 4000);
    }
// Show notification for command result if available
    // Initialize LED states based on server data if available
    {% if led_states %}
        const ledStates = {{ led_states|safe }};
        for (const ledNum in ledStates) {
            updateLEDVisual(ledNum, ledStates[ledNum]);
            localStorage.setItem(`led${ledNum}State`, ledStates[ledNum]);
        }
    {% endif %}
    
    {% if message %}
        {% if 'failed' in message %}
            showNotification('{{ message }}', 'error');
        {% else %}
            showNotification('{{ message }}', 'success');
        {% endif %}
    {% endif %}
</script>
