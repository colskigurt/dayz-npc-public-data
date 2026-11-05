# 4파트14클립 CPU 대조 검토 안내

4파트 전체14클립 네이티브 변환·CPU 포즈 대조를 단일 서버 실행에서 통과했다. 331본의 정렬 구간 변환·루트 차이0. 게임플레이·시각·성능 수용은 별도 미검증이다.

[상세](reports/operations/four-part-fullclip-native-pose-20261106.md) · [데이터](api/public/four-part-fullclip-native-pose.json) · [선정 원시 행](evidence/four-part-fullclip-native-pose-20261106.txt)

미정렬 초기 행 제외,원시 자료 보존. NativeSeconds는 내부 그래프 시계가 아니다. 원시 헤더 누락은 보존된 출력식으로 해석했고 원인은 미확인이다. 완료1회와 컴파일 실패1회를 독립 반복2회로 세지 않는다. 실패한 서버의 강제 정리와 완료 서버의 실제 exit=-1을 정상0종료로 바꾸지 않았다.

다음 단일 산출물: 기존 게임플레이 그래프를 유지한 격리4파트 네트워크 생성 어댑터. 이후8→64기능·화면·반복 비용 비교가 남는다. 기존 물리 검사 삭제·강제 지지 성공을 하지 않는다.

[직전 안내](reports/history/20261106-before-four-fullpose-AI_REVIEW.md) · [기존 실험](reports/latest.md)
