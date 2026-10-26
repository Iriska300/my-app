pipeline {
    agent any

    environment {
        REGISTRY     = "ВАШ_ЛОГИН"
        DOCKER_CREDS = "docker-hub-credentials"
        KUBECONFIG   = "/var/lib/jenkins/.kube/config"
    }

    stages {
        stage('Checkout') {
            steps { checkout scm }
        }

        stage('Build & Push images') {
            parallel {
                stage('Client') {
                    steps {
                        script {
                            def tag = "${env.BUILD_NUMBER}-${env.GIT_COMMIT.take(7)}"
                            docker.withRegistry('', env.DOCKER_CREDS) {
                                def img = docker.build("${env.REGISTRY}/client:${tag}", "./client")
                                img.push()
                                img.push('latest')
                            }
                        }
                    }
                }
                stage('Server') {
                    steps {
                        script {
                            def tag = "${env.BUILD_NUMBER}-${env.GIT_COMMIT.take(7)}"
                            docker.withRegistry('', env.DOCKER_CREDS) {
                                def img = docker.build("${env.REGISTRY}/server:${tag}", "./server")
                                img.push()
                                img.push('latest')
                            }
                        }
                    }
                }
            }
        }

        stage('Deploy to k3s') {
            steps {
                sh """
                    TAG="${env.BUILD_NUMBER}-${env.GIT_COMMIT.take(7)}"
                    
                    helm upgrade --install server ./helm/server-chart \
                        --set image.repository=${env.REGISTRY}/server \
                        --set image.tag=\$TAG \
                        --wait --timeout 3m

                    helm upgrade --install client ./helm/client-chart \
                        --set image.repository=${env.REGISTRY}/client \
                        --set image.tag=\$TAG \
                        --wait --timeout 3m
                """
            }
        }

        stage('Verify') {
            steps {
                sh """
                    kubectl get pods
                    kubectl get svc
                    helm list
                """
            }
        }
    }

    post {
        success { echo "Deployed successfully" }
        failure { echo "Pipeline failed." }
    }
}
