# DayZ NPC — 작업 기록·공개 연구 자료

앞으로 이 GitHub 저장소(B안)만 사용합니다. GPT 대시보드는 운영을 종료합니다.

## 먼저 읽을 문서

- [최신 작업 완료 기록](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations-latest.md)
- [최신 실험 요약](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/latest.md)
- [AI에게 전달할 단일 검토 문서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/AI_REVIEW.md)
- [게시·접근 검증 상태](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/verification/latest.json)

## 공개 데이터와 근거

- [상태 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/status.json)
- [실험 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/experiments.json)
- [지표 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/metrics.json)
- [기능 함수·대표 로그](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/downloads/admission-review.txt)
- [애니메이션 함수·대표 로그](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/downloads/animation-review.txt)
- [파일 SHA-256 목록](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/manifest.json)

## 갱신 원칙

의미 있는 작업/실험 묶음이 안전하게 끝나면 로컬 체크포인트를 작성하고, 공개 가능한 JSON·보고서·선정된 함수/로그 발췌를 이 저장소에 갱신합니다. 완료 답변에는 해당 B안 링크와 고정 커밋 링크를 함께 제공합니다. 새 도구/피드백/누락·보완점/다음 실험 계획도 보고서에 포함합니다.

작업 완료 기록과 게임 실험 결과는 구분합니다. generatedAt/exportedAt은 새 게임 실험 수행 시각이 아닙니다. 현재 UTC가 자료의 staleAfter를 넘으면 오래된 자료입니다. 진단 성공·실험 성공·최종 수용을 따로 판정하고, 일반 HTTP200을 모든 ChatGPT 세션의 읽기 성공으로 간주하지 않습니다.

## 공개 범위

전체 원시 로그·영상이 아닌 선정된 공개 자료입니다. 원본 설치 자산, 비공개 원시 기록, 게임 코드 전체, 자격증명, 로컬 경로·프로세스 정보는 포함하지 않습니다. 영상 품질이나 GPU/Present 시각을 텍스트만으로 검증했다고 주장하지 않습니다.

JSON의 `/downloads/...` 같은 canonical URI는 `https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main` 뒤에 붙여 해석합니다. 과거 보고서의 이전 대시보드 언급은 당시 이력이며 현재 진입점이 아닙니다. 현재 ChatGPT 웹 도구의 새 Raw URL Cache miss는 검증 기록에 미해결로 남깁니다.
