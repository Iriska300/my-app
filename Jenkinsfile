pipeline {
    agent any

    environment {
        REGISTRY     = "ghcr.io/Iriska300"
        IMAGE_TAG    = "\${env.BUILD_NUMBER}-\${env.GIT_COMMIT.take(7)}"
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
                            docker.withRegistry('https://ghcr.io', env.DOCKER_CREDS) {
                                def img = docker.build("\${env.REGISTRY}/client:\${env.IMAGE_TAG}", "./client")
                                img.push()
                                img.push('latest')
                            }
                        }
                    }
                }
                stage('Server') {
                    steps {
                        script {
                            docker.withRegistry('https://ghcr.io', env.DOCKER_CREDS) {
                                def img = docker.build("\${env.REGISTRY}/server:\${env.IMAGE_TAG}", "./server")
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
                    helm upgrade --install server ./helm/server-chart \
                        --set image.repository=${env.REGISTRY}/server \
                        --set image.tag=${env.IMAGE_TAG} \
                        --wait --timeout 3m

                    helm upgrade --install client ./helm/client-chart \
                        --set image.repository=${env.REGISTRY}/client \
                        --set image.tag=${env.IMAGE_TAG} \
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
        success { echo "Deployed version \${env.IMAGE_TAG}" }
        failure { echo "Pipeline failed." }
    }
}
