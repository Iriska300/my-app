pipeline {
    agent any

    triggers {
        pollSCM('H/2 * * * *')
    }

    environment {
        REGISTRY     = "iriska2604"
        DOCKER_CREDS = "docker-hub-credentials"
        KUBECONFIG   = "/var/lib/jenkins/.kube/config"
    }

    stages {
        stage('Clean workspace (before)') {
            steps { cleanWs() }
        }

        stage('Checkout') {
            steps { checkout scm }
        }

        stage('Build & Push images') {
            steps {
                script {
                    def tag = "${env.BUILD_NUMBER}-${env.GIT_COMMIT.take(7)}"
                    env.IMAGE_TAG = tag

                    def buildAndPush = { String svc ->
                        stage("Build ${svc}") {
                            docker.withRegistry('', env.DOCKER_CREDS) {
                                def img = docker.build("${env.REGISTRY}/${svc}:${tag}", "./${svc}")
                                img.push()
                                img.push('latest')
                            }
                        }
                    }

                    parallel(
                        client: buildAndPush('client'),
                        server: buildAndPush('server')
                    )
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
        always  { cleanWs() }
        success { echo "Deployed successfully" }
        failure { echo "Pipeline failed." }
    }
}
