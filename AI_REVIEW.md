# 서버 파트 소유권 검토

5파트 기준 유지. 4파트 비용3쌍은 방향 불일치로 미수용. 이번은15개 핀 PBO의 서버 포즈 소비자 정적 검토이며 새 성능값·게임 실행은 없다. 

[상세](reports/operations/server-pose-consumers-20261106.md) · [TXT](reports/operations/server-pose-consumers-20261106.txt) · [근거](evidence/server-pose-consumers-20261106.txt) · [JSON](api/public/server-pose-consumers.json)

지지/관절/등반 끝은 Leader 본을 사용한다. 모든 파트가 루트 delta를 조회하지만 Drive는 대표만 호출한다. 확인한 스크립트에서 비대표 root 진단값의 게임플레이 소비자를 찾지 못했다. 이것은 네이티브 포즈 평가를 꺼도 된다는 증거가 아니다. NOCOLLISION/부모 연결 실패는 유지한다.

후보는 비대표 관리 파트의 getter와delta 누적만 줄이며 그래프·시계·충돌·지지·대표/피해/퇴역은 보존한다. 아직 미구현·미측정. 짧은1/8 계약·비용 근거 전64장시간 시험을 하지 않는다.

이전 stimulus 파생JSON의 Starting-position variation 문장은6회 offset0과 불일치하여 정정했다. 원본 보존,입력 우선순위/비용 인과 미확인.

[직전 안내](reports/history/20261106-before-server-pose-AI_REVIEW.md)
