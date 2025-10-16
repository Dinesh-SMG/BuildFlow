pipeline {
    agent { label 'linuxgit' }

    environment {
        GIT_REPO = 'https://github.com/Dinesh-SMG/BuildFlow.git'
        BRANCH = 'main'
        // NOTE: Ensure these environment variables are defined in Jenkins global configuration or credentials
        // SONARQUBE_ENV = 'MySonarServer' 
        // SONAR_ORGANIZATION = 'my-org'
        // SONAR_PROJECT_KEY = 'my-project-key'
    }

    stages {

        stage('Clean Workspace') {
            steps {
                echo 'Cleaning workspace before starting build...'
                cleanWs()
            }
        }

        stage('Checkout Code') {
            steps {
                echo 'Cloning the Git repository...'
                git branch: "${BRANCH}", url: "${GIT_REPO}"
            }
        }

        stage('Prepare Tools') {
            steps {
                echo 'Installing required tools...'
                sh '''
                    # Update and install Python3/pip3 if missing
                    if ! command -v pip3 &>/dev/null; then
                        sudo yum install -y python3 python3-pip || true
                    fi

                    # Install cmakelint
                    pip3 install --quiet cmakelint

                    # Install dos2unix
                    if ! command -v dos2unix &>/dev/null; then
                        sudo yum install -y dos2unix || true
                    fi

                    # Install cmake (which includes ctest)
                    if ! command -v cmake &>/dev/null; then
                        sudo yum install -y epel-release || true
                        sudo yum install -y cmake || true
                    fi

                    # Install GCC/G++ compilers for C/C++ build
                    if ! command -v gcc &>/dev/null; then
                        sudo yum install -y gcc gcc-c++ || true
                    fi
                '''
            }
        }

        stage('Lint') {
            steps {
                echo 'Running lint checks on main.c...'
                sh '''
                    if [ -f src/main.c ]; then
                        cmakelint src/main.c > lint_report.txt
                        # Fail build if lint errors found (uncomment if strict)
                        # grep -q "Total Errors: [1-9]" lint_report.txt && exit 1 || true
                    else
                        echo "main.c not found!"
                        exit 1
                    fi
                '''
            }
            post {
                always {
                    archiveArtifacts artifacts: 'lint_report.txt', fingerprint: true
                    fingerprint 'main.c'
                }
            }
        }

        stage('Build') {
            steps {
                echo 'Running build.sh...'
                sh '''
                    if [ -f build.sh ]; then
                        dos2unix build.sh
                        chmod +x build.sh
                        # Assuming build.sh performs: mkdir build, cd build, cmake .., make
                        bash build.sh
                    else
                        echo "build.sh not found!"
                        exit 1
                    fi
                '''
            }
        }

        // --- NEW TEST STAGE ---
        stage('Test') {
            steps {
                echo 'Running unit tests using CTest...'
                // IMPORTANT: CTest must be run from the directory where CMake was executed, 
                // which is conventionally a subdirectory named 'build'.
                sh '''
                    if [ -d build ]; then
                        echo "Executing CTest from the 'build' directory..."
                        cd build
                        ctest --output-on-failure
                    else
                        echo "Build directory not found. Cannot run tests. Did build.sh fail?"
                        exit 1
                    fi
                '''
            }
            // Optional: If your CTest is configured to generate JUnit XML reports, 
            // you can uncomment the post step below to publish the results.
            /*
            post {
                always {
                    junit 'build/TestResults.xml'
                }
            }
            */
        }
        // --- END NEW TEST STAGE ---

        stage('SonarQube Analysis') {
            steps {
                echo 'Running SonarQube (SonarCloud) analysis...'
                withSonarQubeEnv("${SONARQUBE_ENV}") {
                    sh '''
                        sonar-scanner \
                          -Dsonar.organization=${SONAR_ORGANIZATION} \
                          -Dsonar.projectKey=${SONAR_PROJECT_KEY} \
                          -Dsonar.sources=src \
                          -Dsonar.cfamily.compile-commands=compile_commands.json \
                          -Dsonar.host.url=https://sonarcloud.io \
                          -Dsonar.sourceEncoding=UTF-8
                    '''
                }
            }
        }
    }

    post {
        always {
            echo 'Pipeline finished.'
        }
        success {
            // Updated success message to reflect the new stage
            echo 'Clean, Lint, Build, Test, and SonarQube Analysis completed successfully!'
        }
        failure {
            echo 'Pipeline failed. Check logs.'
        }
    }
}
